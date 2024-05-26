#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>

#define RS485_DEVICE "/dev/ttyS5"  // UART device file
#define BAUDRATE B9600             // Baudrate (can be changed as needed)
#define BUFFER_SIZE 255            // Buffer size for sending and receiving data

int fd;

void* receive_thread(void* arg) {
  char receive_buffer[BUFFER_SIZE];

  while (1) {
    // Read data
    int bytes_read = read(fd, receive_buffer, BUFFER_SIZE - 1);
    if (bytes_read > 0) {
      receive_buffer[bytes_read] = '\0';  // Null-terminate the string
      printf("Message received: %s\n", receive_buffer);
      // You can add additional processing for received messages here
    }
  }

  return NULL;
}

int main() {
  struct termios serial;
  char input_buffer[BUFFER_SIZE];
  char send_buffer[BUFFER_SIZE];
  pthread_t thread_id;

  // Open serial port
  fd = open(RS485_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1) {
    perror("Unable to open port");
    return 1;
  }

  // Configure serial port
  memset(&serial, 0, sizeof(serial));
  serial.c_cflag = CS8 | CLOCAL | CREAD;
  serial.c_iflag = IGNPAR;
  serial.c_oflag = 0;
  serial.c_lflag = 0;

  serial.c_lflag &= ~ICANON;
  serial.c_lflag &= ~ISIG;

  serial.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  serial.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

  cfsetspeed(&serial, B9600);
  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &serial);

  // Create receive thread
  if (pthread_create(&thread_id, NULL, receive_thread, NULL) != 0) {
    perror("Error creating receive thread");
    close(fd);
    return 1;
  }

  // Main thread for sending messages
  while (1) {
    // Get user input for message to send
    printf("Enter message to send: ");
    fgets(input_buffer, BUFFER_SIZE, stdin);

    input_buffer[strcspn(input_buffer, "\n")] = '\0';  // Remove newline character

    sprintf(send_buffer, "%s\r\n", input_buffer);

    // Write data
    if (write(fd, send_buffer, strlen(send_buffer)) == -1) {
      perror("Write error");
      close(fd);
      return 1;
    }
    printf("Message sent: %s\n", send_buffer);
  }

  // Close serial port (this part will not be reached in the infinite loop)
  close(fd);

  return 0;
}
