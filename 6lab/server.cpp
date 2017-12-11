#include "bank.h"
#include <iostream>
#include <cstring>
#include "zmq.h"
#include <unistd.h>
#include <string>

typedef struct MD
{
    int clientId;
    int messageNumber;
    char message[128];
    int action;
    int amount;
    int whom;
} MessageData;

int main(int argc, char const *argv[])
{
    void* context = zmq_ctx_new();
    void* serverSocket = zmq_socket(context, ZMQ_REP);
    char address[32];
    int bank_id;
    std::cin >> bank_id;
    sprintf(address, "%s%d", "tcp://*:", bank_id);
    zmq_bind(serverSocket, address);
    std::cout << "Starting..." << std::endl;
    Bank b;
    for (;;)
    {
        zmq_msg_t message;
        zmq_msg_init(&message);
        zmq_msg_recv(&message, serverSocket, 0);
        MessageData *m = (MessageData *)zmq_msg_data(&message);
        printf("Message from client: %d  messageId: %d message: ", m->clientId, m->messageNumber);
        std::cout <<  m->message << std::endl;
        zmq_msg_close(&message);

        b.add_new_client(m->clientId);

        zmq_msg_t reply;
        char answer[128];

        switch(m->action) {
            case 1:
                if (b.get_money(m->clientId, m->amount)) {
                    sprintf(answer, "Here is your %d dollars", m->amount);
                    std::cout << "I`ve just given " << m->amount << " dollars to " << m->clientId << "..." << std::endl;
                } else {
                    sprintf(answer, "You don`t have enough money");
                    std::cout << "Haha, " << m->clientId << " doesn`t have " << m->amount << " dollars!" << std::endl;
                }
                break;
            case 2:
                b.add_money(m->clientId, m->amount);
                sprintf(answer, "Thank you for %d dollars", m->amount);
                std::cout << m->clientId << " has just put " << m->amount << " dollars into me!" << std::endl;
                break;
            case 3:
                sprintf(answer, "Your balance is %d", b.balance(m->clientId));
                std::cout << m->clientId << " has just requested his balance" << std::endl;
                break;
            case 4:
                if (b.balance(m->clientId) >= m->amount) {
                    b.get_money(m->clientId, m->amount);
                    b.add_money(m->whom, m->amount);
                    sprintf(answer, "Done");
                    std::cout << "Transfered " << m->amount << " dollars from " << m->clientId << " to " << m->whom << std::endl;
                } else {
                    sprintf(answer, "You don`t have enough money");
                    std::cout << m->clientId << " doesn`t have " << m->amount << " dollars to transfer them to " << m->whom << std::endl;
                }
                break;
        }

        zmq_msg_init_size(&reply, strlen(answer) + 1);
        memcpy(zmq_msg_data(&reply), answer, strlen(answer) + 1);
        zmq_msg_send(&reply, serverSocket, 0);
        zmq_msg_close(&reply);
    }
    zmq_close(serverSocket);
    zmq_ctx_destroy(context);

    return 0;
}
