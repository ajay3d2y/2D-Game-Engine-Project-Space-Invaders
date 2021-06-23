#include <Game.h>
#include <zmq.hpp>
#include <msgpack.hpp>
#include <thread>

using namespace msgpack;

class ClientThread {
	
public:
	ClientThread(Game *g1,int duty) {
		gameController = g1;
		if (duty == 0) {
			port = "tcp://localhost:5551";
		}
		else{
			port = "tcp://localhost:5556";
		}
		job = duty;
	}

	void run() {
		if (job == 0) {
			zmq::context_t context(1);
			zmq::socket_t socket(context, ZMQ_REQ);
			socket.connect(port);
			sbuffer firstBuffer;
			pack(firstBuffer, "Start");

			zmq::message_t firstContact(firstBuffer.size());
			memcpy(firstContact.data(), firstBuffer.data(), firstBuffer.size());
			socket.send(firstContact, zmq::send_flags::none);

			zmq::message_t firstReply;
			socket.recv(firstReply, zmq::recv_flags::none);
			sbuffer replyBuffer;
			replyBuffer.write(static_cast<const char*>(firstReply.data()), firstReply.size());
			unpacked msg;
			msgpack::unpack(msg, replyBuffer.data(), replyBuffer.size());
			msgpack::object obj = msg.get();
			std::string firstResp;
			obj.convert(firstResp);
			
			std::cout << " Congrats \n";
			while (true) {
				while (gameController->peekEventQueue() == 0) {

				}
				std::string eventStr = gameController->getEvent();
				sbuffer inputBuffer;
				pack(inputBuffer, eventStr);
				
				zmq::message_t inputMessage(inputBuffer.size());
				memcpy(inputMessage.data(), inputBuffer.data(), inputBuffer.size());
				socket.send(inputMessage, zmq::send_flags::none);
				
				zmq::message_t response;
				socket.recv(response, zmq::recv_flags::none);
				
			}

		}
		else {
			zmq::context_t context(1);
			zmq::socket_t subscriber(context, ZMQ_SUB);
			int conflate = 1;
			subscriber.setsockopt(ZMQ_CONFLATE, &conflate, sizeof(conflate));
			subscriber.connect(port);
			subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
			while (true) {
				zmq::message_t msg;
				sbuffer responseBuffer;
				unpacked unpackedMsg;
				subscriber.recv(msg, zmq::recv_flags::none);
				responseBuffer.write(static_cast<const char*>(msg.data()), msg.size());
				msgpack::unpack(unpackedMsg, responseBuffer.data(), responseBuffer.size());
				msgpack::object obj = unpackedMsg.get();
				std::vector<std::string> data;
				obj.convert(data);
				gameController->serverUpdate(data);
			}
			
		}
		

	}

private:
	Game* gameController;
	int target;
	std::string port;
	int job;
};

void run_wrapper(ClientThread* c) {
	c->run();
}

int main() {
	Game gameController;
	ClientThread c1(&gameController,1);
	ClientThread c2(&gameController, 0);
	std::thread subscriber(run_wrapper, &c1);
	std::thread requester(run_wrapper, &c2);
	gameController.run();
	subscriber.join();
	requester.join();
	return 0;
}