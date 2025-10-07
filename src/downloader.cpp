#include "downloader.hpp"

struct Url {
	std::string scheme, host, port, target;
	inline bool https() const { return scheme == "https"; }
};

static Url parse_url(std::string url) {
	Url u;
	auto pos = url.find("://");
	if (pos == std::string::npos) throw std::runtime_error("Bad URL: scheme missing");
	u.scheme = url.substr(0, pos);
	auto rest = url.substr(pos + 3);
	auto slash = rest.find('/');
	auto hostport = slash == std::string::npos ? rest : rest.substr(0, slash);
	u.target = slash == std::string::npos ? "/" : rest.substr(slash);
	auto colon = hostport.find(':');
	if (colon == std::string::npos) {
		u.host = hostport;
		u.port = (u.scheme == "https") ? "443" : "80";
	} else {
		u.host = hostport.substr(0, colon);
		u.port = hostport.substr(colon + 1);
	}
	if (u.scheme != "http" && u.scheme != "https")
		throw std::runtime_error("Only http/https are supported");
	return u;
}

bool download_file(const std::string& url_str, const std::string& out_path) {
	try {
		auto url = parse_url(url_str);
		net::io_context ioc;
		
		auto host_header = url.host;
		if ((!url.https() && url.port != "80") || (url.https() && url.port != "443")) host_header += ":" + url.port;
		
		http::request<http::empty_body> req{http::verb::get, url.target, 11};
		req.set(http::field::host, host_header);
		req.set(http::field::user_agent, std::string("asio-beast-downloader/") + BOOST_BEAST_VERSION_STRING);
		req.set(http::field::accept, "*/*");
		req.set(http::field::accept_encoding, "identity");
		req.set(http::field::connection, "close");
		
		beast::error_code ec;
		beast::flat_buffer buffer;
		
		if (!url.https()) {
			// HTTP
			tcp::resolver resolver{ioc};
			beast::tcp_stream stream{ioc};
			
			auto results = resolver.resolve(url.host, url.port);
			stream.connect(results, ec);
			if (ec) return true;
			
			http::write(stream, req, ec);
			if (ec) return true;
			
			http::response_parser<http::file_body> parser;
			parser.get().body().open(out_path.c_str(), beast::file_mode::write, ec);
			if (ec) return true;
			
			http::read(stream, buffer, parser, ec);
			if (ec) return true;
			
			auto const& res = parser.get();
			if (res.result() != http::status::ok) return true;
			
			stream.socket().shutdown(tcp::socket::shutdown_both, ec);
			
			return false;
		} else {
			// HTTPS (TLS)
			ssl::context ctx{ssl::context::tls_client};

			ctx.set_default_verify_paths();
			
			beast::ssl_stream<beast::tcp_stream> stream{ioc, ctx};

			stream.set_verify_mode(ssl::verify_peer);
			stream.set_verify_callback(ssl::host_name_verification(url.host));
			
			if(!SSL_set_tlsext_host_name(stream.native_handle(), url.host.c_str())) return true;
			
			tcp::resolver resolver{ioc};
			auto results = resolver.resolve(url.host, url.port, ec);
			if (ec) return true;
			
			beast::get_lowest_layer(stream).connect(results, ec);
			if (ec) return true;
			
			stream.handshake(ssl::stream_base::client, ec);
			if (ec) return true;
			
			http::write(stream, req, ec);
			if (ec) return true;
			
			http::response_parser<http::file_body> parser;
			parser.get().body().open(out_path.c_str(), beast::file_mode::write, ec);
			if (ec) return true;
			
			http::read(stream, buffer, parser, ec);
			if (ec) return true;
			
			auto const& res = parser.get();
			if (res.result() != http::status::ok) return true;
			
			stream.shutdown(ec);
			
			return false;
		}
	} catch (...) {
		return true;
	}
}