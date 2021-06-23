#include <Game.h>
#include <zmq.hpp>
#include <msgpack.hpp>
#include <thread>

std::string filePath = "C:\\Users\\Ajay\\Downloads";
class Process {
public:
	
	Process(Game *gc1,int duty) {
		gc = gc1;
		job = duty;
	}

	void run() {
		if (job == 0) {
			zmq::context_t context(1);
			zmq::socket_t responder(context, ZMQ_REP);
			responder.bind("tcp://*:5551");

			while (true) {
				zmq::message_t recievedMsg;
				responder.recv(recievedMsg, zmq::recv_flags::none);
				msgpack::sbuffer buffer;
				buffer.write(static_cast<const char*>(recievedMsg.data()), recievedMsg.size());

				msgpack::unpacked unpackedMsg;
				msgpack::unpack(unpackedMsg, buffer.data(), buffer.size());
				msgpack::object obj = unpackedMsg.get();
				std::string msg;
				obj.convert(msg);

				std::string rmsg;
				if (msg.compare("Start") == 0) {
					rmsg = "Started";
				}
				else {
					/*gc->setDirection(msg.at(0)-48, msg.at(2));*/
					gc->recvEvent(msg);
					rmsg = "Done";
				}

				msgpack::sbuffer buffer1;
				msgpack::pack(buffer1, rmsg);

				zmq::message_t replymsg(buffer1.size());
				memcpy(replymsg.data(), buffer1.data(), buffer1.size());
				responder.send(replymsg, zmq::send_flags::none);
			}
		}
		else {
			zmq::context_t context(1);
			zmq::socket_t publisher(context, ZMQ_PUB);
			publisher.bind("tcp://*:5556");
			while (true) {
				std::vector<std::string> data = gc->getData();
				msgpack::sbuffer pBuffer;
				msgpack::pack(pBuffer, data);

				zmq::message_t msg(pBuffer.size());
				memcpy(msg.data(), pBuffer.data(), pBuffer.size());
				publisher.send(msg, zmq::send_flags::none);
			}
		}


	}

	

private:
	Game* gc;
	int job;
};

void run_wrapper(Process* p) {
	p->run();
}

int main(){
	Game gameController;
	Process p1(&gameController,1);
	Process p2(&gameController, 0);
	std::thread publish(run_wrapper, &p1);
	std::thread response(run_wrapper, &p2);
	gameController.run();
	publish.join();
	response.join();
	return 0;
	}

	