# Socket_TCP
 
# SendData Readme

This README provides instructions for running the "SendData" program, which allows you to send text or files to a remote destination using sockets. The program is designed to run from the command line with different options based on the type of data you want to send.

## Prerequisites

Before you can use the "SendData" program, make sure you have the following prerequisites:

- C++ compiler (e.g., g++)
- Linux environment (or compatible with POSIX sockets)
- Basic understanding of command-line usage

## Compilation

To compile the program, use the following command:

```bash
g++ SendData.cpp -o SendData
```

## Usage

The program can be used to send text or files to a remote destination. It accepts the following command-line arguments:

```bash
./SendData <destination_address> <destination_port> <options>
```

- `<destination_address>`: The IP address or hostname of the remote destination.
- `<destination_port>`: The port number to establish a connection.
- `<options>`: Specify the operation and additional arguments.

### Sending Text

To send text to the remote destination, use the following command:

```bash
./SendData <destination_address> SendText "<text_to_send>"
```

- `<text_to_send>`: The text you want to send (enclose in double quotes if it contains spaces or special characters).

Example:

```bash
./SendData 192.168.1.100 SendText "Hello, this is a text message."
```

### Sending Files

To send a file to the remote destination, use the following command:

```bash
./SendData <destination_address> SendFile "<file_path>" <buffer_size>
```

- `<file_path>`: The path to the file you want to send (enclose in double quotes if it contains spaces or special characters).
- `<buffer_size>`: The buffer size to read and send the file in chunks (e.g., 1024).

Example:

```bash
./SendData 192.168.1.100 SendFile "file.txt" 1024
```

## Important Notes

- Ensure that the destination address and port are correctly specified.
- Make sure the file you want to send exists and is accessible.
- Use a suitable buffer size when sending files to optimize performance.
- Handle any firewall or security settings that may affect network communication.

## Error Handling

The program provides error messages for common issues, such as invalid addresses, failed connections, or file not found errors.

## Troubleshooting

If you encounter any issues while running the program, double-check the command-line arguments and ensure that the destination server is reachable and listening on the specified port.

For additional assistance, please refer to the program's source code and comments for further details on its implementation.




# ReceiveData Readme

This README provides instructions for running the "ReceiveData" program, which allows you to receive files from a remote sender using sockets. The program is designed to run from the command line with specific options.


## Compilation

To compile the program, use the following command:

```bash
g++ ReceiveData.cpp -o ReceiveData
```

## Usage

The program can be used to receive files sent by a remote sender. It accepts the following command-line arguments:

```bash
./ReceiveData -out <location_to_store_file> 
```

- `-out`: This flag specifies the location where the received file will be stored.
- `<location_to_store_file>`: The path and filename to save the received file.

Example:

```bash
./ReceiveData -out /path/...
```

## Important Notes

- Ensure that the directory where you intend to save the received file exists and is writable.
- The program will listen for incoming connections on the specified port and save the received file to the specified location.

## Error Handling

The program provides error messages for common issues, such as socket creation failure, binding failure, listening failure, or acceptance failure. These messages will help you identify and troubleshoot any problems that may arise during execution.

## Troubleshooting

If you encounter any issues while running the program, double-check the command-line arguments and ensure that the sender has correctly specified the destination address and port when sending the file.

For additional assistance, please refer to the program's source code and comments for further details on its implementation.