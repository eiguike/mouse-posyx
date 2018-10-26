#include "server.h"
#include "protocols/about_protocol.h"
#include "logger.h"

#define APPLICATION_VERSION "mouse-posyx 1.0v"

INFORMATION Computer;
BUFFER Buffer;

int CallbackAbout( struct lws *wsi,
    enum lws_callback_reasons reason,
    void *user,
    void *in,
    size_t len )
{
  switch(reason) {
    case LWS_CALLBACK_SERVER_WRITEABLE:
      Logger->Info("Sending message...");
      lws_write( wsi,
                 &received_payload.data[LWS_SEND_BUFFER_PRE_PADDING],
                 received_payload.len,
                 LWS_WRITE_TEXT );
      break;
    default:
      break;
  }

  return 0;
}

int SizeOfInformation () {
  int Acumulator = 0;

  Acumulator += strlen(Computer.Name);
  Acumulator += strlen(Computer.Linux);
  Acumulator += strlen(Computer.ApplicationVersion);

  return Acumulator;
}

void InitializeAboutProtocol () {
  FILE *ComputerInformation = popen("uname -a", "r");
  char Buffer[256];
  char * Pointer = NULL;

  while (fgets(Buffer, sizeof(Buffer), ComputerInformation) != 0) {
    strtok(Buffer, " ");

    Pointer = strtok(NULL, " ");
    Computer.Name = malloc(sizeof((strlen(Pointer) + 1) * char));
    strcpy (Computer.Name, Pointer);

    Pointer = strtok(NULL, " ");
    Computer.Linux = malloc(sizeof((strlen(Pointer) + 1) * char));
    strcpy (Computer.Linux, Pointer);

    Computer.ApplicationVersion = malloc(sizeof((strlen(APPLICATION_VERSION) + 1) * char));
    strcpy (Computer.ApplicationVersion, APPLICATION_VERSION);
  }

  pclose(ComputerInformation);
}
