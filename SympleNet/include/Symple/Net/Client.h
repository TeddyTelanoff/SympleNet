#pragma once

#include "Symple/Net/Connection.h"

namespace Symple::Net
{
	template<typename T>
	class Client
	{
	private:
		ThreadSafeQueue<OwnedMessage<T>> m_RecievedMessages;
	protected:
		asio::io_context m_Context;
		std::thread m_ContextThread;
		asio::ip::tcp::socket m_Socket;

		std::unique_ptr<Connection<T>> m_Connection;
	public:
		Client()
			: m_Socket(m_Context) {}

		virtual ~Client()
		{ Disconnect(); }

		bool Connect(std::string_view host, uint16_t port)
		{
			try
			{
				m_Connection = std::make_unique<Connection<T>>();

				asio::ip::tcp::resolver resolver(m_Context);
				m_EndPoints = resolver.resolve(host, std::to_string(port));

				m_Connection->ConnectToServer(m_EndPoints);
				m_ContextThread = std::thread([this]() { m_Context.run(); });

				return true;
			}
			catch (std::exception &e)
			{
				std::cerr << "Client exception: " << e.what() << '\n';
				return false;
			}
		}

		void Disconnect()
		{
			if (IsConnected())
				m_Connection->Disconnect();

			m_Context.stop();
			if (m_ContextThread.joinable())
				m_ContextThread.join();

			m_Connection.release();
		}

		bool IsConnected() const
		{ return m_Connection && m_Connection->IsConnected(); }

		ThreadSafeQueue<OwnedMessage<T>> &IncomingMessages()
		{ return m_RecievedMessages; }

		const ThreadSafeQueue<OwnedMessage<T>> &IncomingMessages() const
		{ return m_RecievedMessages; }
	};
}