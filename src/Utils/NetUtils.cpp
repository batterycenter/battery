
#include "Battery/pch.h"
#include "Battery/Core/Exception.h"
#include "Battery/Log/Log.h"
#include "Battery/Utils/NetUtils.h"
#include "Battery/StringUtils.h"
#include "Battery/Utils/TimeUtils.h"
#include "Battery/Utils/FileUtils.h"

#ifndef BATTERYENGINE_DISABLE_NETWORKING
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "Battery/Extern/httplib.h"
#endif

#include "Battery/Extern/magic_enum.hpp"

#define ASIO_STANDALONE
#include "Battery/AllegroDeps.h"		// -> Allegro sets some WINNT defines needed for asio/winsock
#include <asio.hpp>
using asio::ip::udp;

#pragma warning( disable : 4101 )		// Suppress warning about unused variable 'e' (Only in this .cpp file)

namespace Battery {

	// TODO: Fix UDPServer and Client with the IncompleteTypeWrapper, also for RobotinoLib

	// UDPServer

	struct UDPServerData {

		asio::io_service ioService;
		std::unique_ptr<udp::socket> socket;
		udp::endpoint remoteEndpoint;

		bool terminate = false;
		std::unique_ptr<std::thread> listenerThread;
		std::function<void(uint8_t* packet, size_t packetSize)> callback;

		std::unique_ptr<uint8_t[]> buffer;
		size_t bufferSize;

		UDPServerData() = default;
		~UDPServerData() = default;
	};

	// -> UDPServerDataWrapper Child class vvvvvv
	UDPServer::UDPServerDataWrapper::UDPServerDataWrapper() {
		data = new UDPServerData();
	}

	UDPServer::UDPServerDataWrapper::~UDPServerDataWrapper() {
		delete data;
	}

	UDPServerData* UDPServer::UDPServerDataWrapper::get() {
		return data;
	}
	// -> UDPServerDataWrapper Child class ^^^^^^

	UDPServer::UDPServer() {
		try {
			data = std::make_unique<UDPServerDataWrapper>();
			LOG_CORE_TRACE("UDPServer Instance constructed");
		}
		catch (std::exception& e) {
			throw Battery::Exception(std::string("ASIO Exception: ") + e.what());
		}
	}

	UDPServer::UDPServer(std::function<void(uint8_t* packet, size_t packetSize)> callback, uint16_t port, size_t bufferSize) {
		try {
			data = std::make_unique<UDPServerDataWrapper>();
			LOG_CORE_TRACE("UDPServer Instance constructed");
		}
		catch (std::exception& e) {
			throw Battery::Exception(std::string("ASIO Exception: ") + e.what());
		}
		Listen(callback, port, bufferSize);
	}

	UDPServer::~UDPServer() {
		LOG_CORE_TRACE("Terminating UDP listener");

		// Set the terminate flag and wait until the listener thread returns
		data->get()->terminate = true;
		data->get()->socket->close();
		data->get()->listenerThread->join();
		data->get()->listenerThread.reset();

		LOG_CORE_TRACE("UDPServer Instance destructed");
	}

	void UDPServer::Listen(std::function<void(uint8_t* packet, size_t packetSize)> callback, uint16_t port, size_t bufferSize) {
		try {
			LOG_CORE_TRACE("Creating UDP listener");

			// Setting all data members
			data->get()->socket = std::make_unique<udp::socket>(data->get()->ioService, udp::endpoint(udp::v4(), port));
			data->get()->bufferSize = bufferSize;
			data->get()->callback = callback;

			// Initialize the buffer
			data->get()->buffer.reset(new uint8_t[bufferSize]);
			for (size_t i = 0; i < bufferSize; i++) {
				data->get()->buffer[i] = 0;
			}

			// Start the listener thread
			data->get()->listenerThread = std::make_unique<std::thread>(std::bind(&UDPServer::ListenerThread, this));

			LOG_CORE_TRACE("UDPServer Instance constructed");
		}
		catch (std::exception& e) {
			throw Battery::Exception(std::string("ASIO Exception: ") + e.what());
		}
	}

	void UDPServer::OnReceive(const std::error_code& error, size_t bytes) {
		if (!error) {

			LOG_CORE_TRACE("UDP Packet received, calling client callback");
			data->get()->callback(data->get()->buffer.get(), bytes);

		}
		else {

			if (data->get()->terminate)	// Errors are ignored if thread is being terminated
				return;

			LOG_CORE_WARN("UDP Error " + std::to_string(error.value()) + ": " + error.message());
		}

		// Start listening for the next packet
		StartAsyncListener();
	}

	void UDPServer::StartAsyncListener() {
		try {
			data->get()->socket->async_receive_from(asio::buffer(data->get()->buffer.get(), data->get()->bufferSize), data->get()->remoteEndpoint,
				std::bind(&UDPServer::OnReceive, this, std::placeholders::_1, std::placeholders::_2));
			LOG_CORE_TRACE("UDP async listener started");
		}
		catch (std::exception& e) {
			throw Battery::Exception(std::string("ASIO Exception: ") + e.what());
		}
	}

	void UDPServer::ListenerThread() {

		LOG_CORE_TRACE("UDP listener thread started");

		try {

			// Start listener once
			StartAsyncListener();

			// Main loop in the listener thread
			while (!data->get()->terminate) {
				data->get()->ioService.run_one();
			}

		}
		catch (Battery::Exception& e) {
			LOG_CORE_CRITICAL(std::string("Battery::Exception from listener thread: ") + e.what());
		}
		catch (std::exception& e) {
			LOG_CORE_CRITICAL(std::string("ASIO UDP Exception from listener thread: ") + e.what());
		}
		catch (...) {
			LOG_CORE_CRITICAL("Unknown exception from listener thread!");
		}

		LOG_CORE_TRACE("UDP listener thread returned");
	}









	// UDPClient

	struct UDPClientData {
		asio::io_service ioService;
		udp::socket socket;
		udp::endpoint remote_endpoint;
		bool endpointSet = false;

		UDPClientData() : socket(ioService) {}
		~UDPClientData() = default;
	};

	// -> UDPClientDataWrapper Child class vvvvvv
		UDPClient::UDPClientDataWrapper::UDPClientDataWrapper() {
			data = new UDPClientData();
		}

		UDPClient::UDPClientDataWrapper::~UDPClientDataWrapper() {
			delete data;
		}

		UDPClientData* UDPClient::UDPClientDataWrapper::get() {
			return data;
		}
	// -> UDPClientDataWrapper Child class ^^^^^^

	UDPClient::UDPClient() {
		try {
			data = std::make_unique<UDPClientDataWrapper>();
			LOG_CORE_TRACE("UDPClient Instance constructed");
		}
		catch (std::exception& e) {
			throw Battery::Exception(std::string("ASIO Exception: ") + e.what());
		}
	}

	UDPClient::UDPClient(UDPClient&& client) {			// Move contructor
		data = std::move(client.data);
	}

	UDPClient::UDPClient(const char* ipAddress, uint16_t port) {
		try {
			data = std::make_unique<UDPClientDataWrapper>();
			data->get()->remote_endpoint = udp::endpoint(asio::ip::address::from_string(ipAddress), port);
			data->get()->endpointSet = true;
			LOG_CORE_TRACE("UDPClient Instance constructed");
		}
		catch (std::exception& e) {
			throw Battery::Exception(std::string("ASIO Exception: ") + e.what());
		}
	}

	UDPClient::~UDPClient() {
		LOG_CORE_TRACE("UDPClient Instance destructed");
	}

	void UDPClient::operator=(UDPClient&& client) {			// Move operator
		data = std::move(client.data);
	}
	
	size_t UDPClient::Send(uint8_t* data, size_t length, const char* ipAddress, uint16_t port) {
		try {
			auto& socket = this->data->get()->socket;

			socket.open(udp::v4());
			size_t bytes = socket.send_to(asio::buffer(data, length), udp::endpoint(asio::ip::address::from_string(ipAddress), port));
			socket.close();

			return bytes;
		}
		catch (std::exception& e) {
			throw Battery::Exception(std::string("ASIO Exception: ") + e.what());
		}
	}

	size_t UDPClient::Send(uint8_t* data, size_t length) {

		if (!this->data->get()->endpointSet) {
			throw Battery::Exception("Can't send UDP data: IP and port were not specified!");
		}

		try {
			auto& socket = this->data->get()->socket;

			socket.open(udp::v4());
			size_t bytes = socket.send_to(asio::buffer(data, length), this->data->get()->remote_endpoint);
			socket.close();

			return bytes;
		}
		catch (std::exception& e) {
			throw Battery::Exception(std::string("ASIO Exception: ") + e.what());
		}
	}

	size_t UDPClient::Send(const char* data) {
		return Send((uint8_t*)data, strlen(data));
	}

	size_t UDPClient::Send(const std::string& data) {
		return Send(data.c_str());
	}











	// =================================================
	// ===      File download / HTTP Utilities       ===
	// =================================================

	std::pair<std::string, std::string> SplitUrl(const std::string& url) {
		std::vector<std::string> urlFragments = Battery::StringUtils::SplitString(url, '/');

		if (urlFragments.size() < 3)
			return std::make_pair("", "");

		std::string hostname = urlFragments[0] + "//" + urlFragments[1];
		std::string pathname = "/";

		for (size_t i = 2; i < urlFragments.size(); i++) {
			pathname += urlFragments[i] + "/";
		}
		pathname.pop_back();

		return std::make_pair(hostname, pathname);
	}



#ifndef BATTERYENGINE_DISABLE_NETWORKING
	std::optional<httplib::Response> GetHttpRequestRaw(const std::string& url, bool followRedirect = true) {

		LOG_CORE_TRACE(__FUNCTION__"(): Accessing {}", url);
		std::pair<std::string, std::string> urlP = SplitUrl(url);

		if (urlP.first.empty() || urlP.second.empty()) {
			LOG_CORE_TRACE(__FUNCTION__"(): Error: Invalid url: server='{}', path='{}'", urlP.first, urlP.second);
			return std::nullopt;
		}

		httplib::Client cli(urlP.first);
		httplib::Result res = cli.Get(urlP.second.c_str());

		if (&res.value() == nullptr || res.error() != httplib::Error::Success) {
			LOG_CORE_TRACE(__FUNCTION__"(): Network failed");
			return std::nullopt;
		}
		LOG_CORE_TRACE(__FUNCTION__"(): Success");

		if (res->status == 302 && followRedirect) {
			std::string follow = res->get_header_value("location");
			LOG_CORE_TRACE(__FUNCTION__"(): Following redirect to {}", follow);
			return GetHttpRequestRaw(follow);
		}

		return std::make_optional(res.value());
	}

	std::optional<HttpResponse> GetHttpRequest(const std::string& url, bool followRedirect) {
		std::optional<httplib::Response> res = GetHttpRequestRaw(url, followRedirect);
		if (!res.has_value())
			return std::nullopt;

		return std::make_optional(HttpResponse(res->body, (size_t)res->status, res->reason));
	}

	std::optional<HttpResponse> GetHttpRequestChunkedImpl(const std::string& url,
		std::function<bool(const char*, size_t)> onReceiveCallback,
		std::optional<std::function<bool(uint64_t, uint64_t)>> onProgressCallback)
	{
		LOG_CORE_TRACE(__FUNCTION__"(): Accessing {}", url);
		std::pair<std::string, std::string> urlP = SplitUrl(url);

		if (urlP.first.empty() || urlP.second.empty()) {
			LOG_CORE_TRACE(__FUNCTION__"(): Error: Invalid url: server='{}', path='{}'", urlP.first, urlP.second);
			return std::nullopt;
		}

		httplib::Client cli(urlP.first);
		std::optional<httplib::Result> res;

		if (onProgressCallback.has_value()) {
			res = cli.Get(urlP.second.c_str(),
				[&](const char* data, size_t bytes) {
				return onReceiveCallback(data, bytes);
			},
				[&](uint64_t progress, uint64_t total) {
				return onProgressCallback.value()(progress, total);
			}
			);
		}
		else {
			res = cli.Get(urlP.second.c_str(),
				[&](const char* data, size_t bytes) {
				return onReceiveCallback(data, bytes);
			},
				[&](uint64_t progress, uint64_t total) {
				return true;
			}
			);
		}

		if (&res->value() == nullptr || res->error() != httplib::Error::Success) {
			LOG_CORE_TRACE(__FUNCTION__"(): Network failed");
			return std::nullopt;
		}

		LOG_CORE_TRACE(__FUNCTION__"(): Success");
		return std::make_optional(HttpResponse(res.value()->body, res.value()->status, res.value()->reason));
	}

	// TODO: Implement max redirect count
	std::optional<HttpResponse> GetHttpRequestChunked(
		const std::string& url,
		std::function<void()> onClearDataCallback,
		std::function<bool(const char*, size_t)> onReceiveCallback,
		std::optional<std::function<bool(uint64_t, uint64_t)>> onProgressCallback,
		bool followRedirect)
	{
		// Download the resource
		std::optional<HttpResponse> res = GetHttpRequestChunkedImpl(url, onReceiveCallback, onProgressCallback);
		if (!res.has_value()) {
			return std::nullopt;
		}

		// Handle redirect
		if (res->status == 302 && followRedirect) {
			LOG_CORE_TRACE(__FUNCTION__"(): Got redirected, requesting again");

			// Send the same request again without redirect
			std::optional<httplib::Response> res = GetHttpRequestRaw(url, false);
			if (!res.has_value()) {
				LOG_CORE_TRACE(__FUNCTION__"(): Second request failed");
				return std::nullopt;
			}

			// Now read the location of the redirect
			std::string follow = res->get_header_value("location");
			LOG_CORE_TRACE(__FUNCTION__"(): Followed redirect to {}, clearing data and restarting download", follow);
			onClearDataCallback();

			// And download the actual resource recursively (allowing several redirects)
			return GetHttpRequestChunked(follow, onClearDataCallback, onReceiveCallback, onProgressCallback);
		}

		return res;
	}

	std::string DownloadUrlToBuffer(
		const std::string& url, std::optional<std::function<bool(uint64_t, uint64_t)>> onProgressCallback,
		bool followRedirect) {

		std::string buffer = "";

		std::optional<HttpResponse> res = GetHttpRequestChunked(url,
			[&]() {
			buffer.clear();
		},
			[&](const char* data, size_t bytes) {
			buffer.append(data, bytes);
			return true;
		},
			onProgressCallback,
			true
			);

		if (!res.has_value())
			return "";

		return buffer;
	}

	bool DownloadUrlToFile(
		const std::string& url, const std::string& targetFile, bool binary,
		std::optional<std::function<bool(uint64_t, uint64_t)>> onProgressCallback,
		bool followRedirect) {

		LOG_CORE_TRACE(__FUNCTION__"(): Preparing directory and opening target file");
		Battery::PrepareDirectory(Battery::GetParentDirectory(targetFile));
		ALLEGRO_FILE* file = al_fopen(targetFile.c_str(), (binary ? "wb" : "w"));
		if (file == nullptr) {
			LOG_CORE_TRACE(__FUNCTION__"(): File failed, memory error...");
			return false;
		}

		std::optional<HttpResponse> res = GetHttpRequestChunked(url,
			[&]() {
				// Clear the file: Close and open it again
				al_fclose(file);
				file = al_fopen(targetFile.c_str(), (binary ? "wb" : "w"));
				if (file == nullptr) {
					LOG_CORE_TRACE(__FUNCTION__"(): File failed, memory error...");
					return false;
				}
				return true;
			},
			[&](const char* data, size_t bytes) {
				if (file)
					al_fwrite(file, data, bytes);

				return true;
			},
			onProgressCallback,
			true
		);

		al_fclose(file);

		if (!res.has_value()) {
			LOG_CORE_TRACE(__FUNCTION__"(): Download failed, removing file from disk");
			Battery::RemoveFile(targetFile);
			return false;
		}

		return true;
	}


#else

	std::optional<HttpResponse> GetHttpRequest(const std::string& url, bool followRedirect) {
		throw std::logic_error("Feature disabled");
	}

	std::optional<HttpResponse> GetHttpRequestChunked(
		const std::string& url,
		std::function<void()> onClearDataCallback,
		std::function<bool(const char*, size_t)> onReceiveCallback,
		std::optional<std::function<bool(uint64_t, uint64_t)>> onProgressCallback,
		bool followRedirect) {
		throw std::logic_error("Feature disabled");
	}

	std::string DownloadUrlToBuffer(
		const std::string& url, std::optional<std::function<bool(uint64_t, uint64_t)>> onProgressCallback,
		bool followRedirect) {
		throw std::logic_error("Feature disabled");
	}

	bool DownloadUrlToFile(
		const std::string& url, const std::string& targetFile, bool binary,
		std::optional<std::function<bool(uint64_t, uint64_t)>> onProgressCallback,
		bool followRedirect) {
		throw std::logic_error("Feature disabled");
	}
#endif
}
