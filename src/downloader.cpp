#include "downloader.hpp"

struct Url {
	std::string scheme, host, port, target;
	[[nodiscard]] constexpr bool https() const { return scheme == "https"; }
};

static std::optional<Url> parse_url(const std::string& url_str) {
	Url u;
	std::string_view url_view(url_str);
	
	const auto scheme_end = url_view.find("://");
	if (scheme_end == std::string_view::npos) return std::nullopt;
	u.scheme = url_view.substr(0, scheme_end);
	if (u.scheme != "http" && u.scheme != "https") return std::nullopt;
	
	url_view.remove_prefix(scheme_end + 3);
	
	const auto authority_end = url_view.find('/');
	u.target = (authority_end == std::string_view::npos) ? "/" : std::string(url_view.substr(authority_end));
	const auto authority = (authority_end == std::string_view::npos) ? url_view : url_view.substr(0, authority_end);
	
	const auto port_pos = authority.find(':');
	if (port_pos == std::string_view::npos) {
		u.host = std::string(authority);
		u.port = u.https() ? "443" : "80";
	} else {
		u.host = std::string(authority.substr(0, port_pos));
		u.port = std::string(authority.substr(port_pos + 1));
	}
	
	return u;
}

template<class Stream>
static http::response<http::file_body> perform_request(Stream& stream, const Url& url, const std::string& out_path) {
	http::response_parser<http::file_body> parser;
	
	beast::error_code ec;
	parser.get().body().open(out_path.c_str(), beast::file_mode::write, ec);
	if (ec) throw beast::system_error{ec};
	
	auto host_header = url.host;
	if ((!url.https() && url.port != "80") || (url.https() && url.port != "443")) {
		host_header += ":" + url.port;
	}
	
	http::request<http::empty_body> req{http::verb::get, url.target, 11};
	req.set(http::field::host, host_header);
	req.set(http::field::user_agent, std::string("asio-beast-downloader/") + BOOST_BEAST_VERSION_STRING);
	req.set(http::field::connection, "close");
	
	http::write(stream, req);
	
	beast::flat_buffer buffer;
	http::read(stream, buffer, parser);
	
	return parser.release();
}

constexpr int MAX_REDIRECTS = 2;

bool download_file(const std::string& url_str, const std::string& out_path, const int redirect_count) {
	if (redirect_count > MAX_REDIRECTS) return true;
	try {
		const auto url_opt = parse_url(url_str);
		if (!url_opt) return true;
		const auto& url = *url_opt;
		
		net::io_context ioc;
		tcp::resolver resolver{ioc};
		const auto results = resolver.resolve(url.host, url.port);
		
		http::response<http::file_body> res;
		
		if (!url.https()) {
			beast::tcp_stream stream{ioc};
			stream.connect(results);
			res = perform_request(stream, url, out_path);
			stream.socket().shutdown(tcp::socket::shutdown_both);
		} else {
			ssl::context ctx{ssl::context::tls_client};
			ctx.set_default_verify_paths();
			ctx.set_verify_mode(ssl::verify_peer);
			beast::ssl_stream<beast::tcp_stream> stream{ioc, ctx};
			
			if(!SSL_set_tlsext_host_name(stream.native_handle(), url.host.c_str())) return true;
			
			get_lowest_layer(stream).connect(results);
			stream.handshake(ssl::stream_base::client);
			res = perform_request(stream, url, out_path);
			stream.shutdown();
		}
		
		const auto status_code = res.result();
		const auto status_class = http::to_status_class(status_code);
		
		if (status_class == http::status_class::redirection) {
			if (res.find(http::field::location) == res.end()) return true;
			
			std::string new_url = std::string(res[http::field::location]);
			return download_file(new_url, out_path, redirect_count + 1);
		}
		
		return status_code != http::status::ok;
	} catch (const std::exception& e) {
		return true;
	}
}