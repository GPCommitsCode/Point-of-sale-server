// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>

using namespace std;

int sock;

void signal_handler(int sig)
{
    char msg[100];

    fputs("\nClient terminating!..", stdout);

    sprintf(msg, "-256");
    send(sock, msg, 100, 0);
    close(sock);
    exit(0);
}

int main(int argc, char const *argv[])
{
    int input_response;

    int valread, client_fd, port;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if (argc < 3)
    {
        {
            printf("\nNot Enough arguments\n");
            return -1;
        }
    }

    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "\nSocket creation error \n";
        return -1;
    }

    // info of type of socket to be created
    port = atoi(argv[2]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(port);

    signal(SIGINT, signal_handler);

    while (1)
    {

        cout << "\nPress 1 to open session with server or 0 to exit the program \n";

        cin >> input_response;

        if (input_response == 1)
        {
            // connection with the server
            if ((client_fd = connect(sock, (struct sockaddr *)&serv_addr,
                                     sizeof(serv_addr))) < 0)
            {
                cout << "\nConnection Failed \n";
                return -1;
            }

            cout << "\nConnected with the server!!\n";

            while (1)
            {
                int request_type = 1;
                int number_of_items;
                int upc_code;
                cout << "\nAdd an item(0) or close(1)\n";

                cin >> request_type;
                // enter new items
                if (request_type == 0)
                {
                    cout << "\nEnter UPC code of the item to be added \n";
                    cin >> upc_code;

                    cout << "\nEnter number of items to be added \n";
                    cin >> number_of_items;

                    string request_string = to_string(request_type) + " " + to_string(upc_code) + " " + to_string(number_of_items);

                    write(sock, request_string.c_str(), strlen(request_string.c_str()));

                    memset(buffer, '\0', sizeof(buffer));
                    valread = read(sock, buffer, 1024);

                    cout << "\nResponse from server: " << buffer << "\n";

                    if (buffer[0] == '2')
                    {
                        cout << "\nSeems like server is down!!!\n";
                        cout << "\nClosing down the connection......\n";
                        input_response = 0;
                        break;
                    }
                }
                // close connection
                if (request_type == 1)
                {
                    cout << "\nSending close request to the server!! \n";
                    write(sock, "1", 1);
                    memset(buffer, '\0', sizeof(buffer));
                    read(sock, buffer, 1024);

                    cout << "\nResponse from server: " << buffer << "\n";

                    cout << "\nThanks\n";

                    input_response = 0; // setting input response as the client is closing

                    break;
                }
            }
        }

        if (input_response == 0)
            exit(0);
    }

    return 0;
}