#ifdef METTLE
#include <metal/metal.h>
#include <metal/stddef.h>
#include <metal/stdio.h>
#include <metal/stdlib.h>
#include <metal/string.h>
#include <metal/stdarg.h>  
#include "metalio.h"
#else // METTLE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#endif // METTLE
 
#include "alloc.h"
#include "utils.h"
#include "zos.h"

#include "stcbase.h"
#include "httpserver.h"

#include "myconfig.h"

static int serveDiscovery(HttpService *service, HttpResponse *response) {
  printf("%s ----> Entered \n", __func__);
  HttpServer *server = service->userPointer;
  if (server == NULL) {
    printf("%s Server is NULL", __func__);
    return 0;
  }

  HttpRequest *request = response->request;

  if (!strcmp(request->method, methodGET)) {
    jsonPrinter *p = respondWithJsonPrinter(response);

    setResponseStatus(response, HTTP_STATUS_OK, "OK");
    setContentType(response, "text/json");
    addStringHeader(response, "Server", "jdmfws");
    addStringHeader(response, "Transfer-Encoding", "chunked");
    writeHeader(response);

    jsonStart(p);
    {
      ACEE *addressSpaceACEE = (ACEE *)getAddressSpaceAcee();
      jsonAddLimitedString(p, "user", addressSpaceACEE->aceeuser, 8);

      ASCB *myASCB = getASCB();
      char *ascbJobName = getASCBJobname(myASCB);
      jsonAddLimitedString(p, "jobname", ascbJobName, 8);

      jsonAddInt(p, "port", HTTP_PORT);
    }
    jsonEnd(p);

    finishResponse(response);
  } else {
    printf("%s unsupported metod", __func__);
  }

  printf("%s <---- Exited \n", __func__);
  return 0;
}

static void installDiscoveryService(HttpServer *server) {
  HttpService *discoveryService = makeGeneratedService("Discovery", "/discovery");
  discoveryService->authType = SERVICE_AUTH_NONE;
  registerHttpService(server, discoveryService);
  discoveryService->serviceFunction = serveDiscovery;
  discoveryService->runInSubtask = FALSE;
  discoveryService->paramSpecList =
          makeStringParamSpec("_metaDataVersion", SERVICE_ARG_OPTIONAL, NULL);
  discoveryService->userPointer = server;
}

static void installServices(HttpServer *server) {
  installDiscoveryService(server);
}

#ifdef METTLE
#pragma prolog(MAIN,\
"        BAKR  14,0                               \n\
         LARL  10,&CCN_CSECT                      \n\
         USING &CCN_CSECT,10                      \n\
         STORAGE OBTAIN,LENGTH=1048736,BNDRY=PAGE \n\
         MVC   4(4,1),=C'F1SA'                    \n\
         J     @@MPRLG                            \n\
         LTORG                                    \n\
         DROP  10                                 \n\
@@MPRLG  DS    0H                                 \n\
         LGR   13,1                               \n\
         EREG  1,1                                ")

#pragma epilog(MAIN,\
"        LGR   1,13                               \n\
         LGR   2,15                               \n\
         LARL  10,&CCN_CSECT                      \n\
         USING &CCN_CSECT,10                      \n\
         STORAGE RELEASE,LENGTH=1048736,ADDR=(1)  \n\
         DROP  10                                 \n\
         LGR   15,2                               \n\
         EREG  0,1                                \n\
         PR                                       ")

int MAIN() {
#else
int main() {
#endif

  STCBase *base = (STCBase*) safeMalloc31(sizeof(STCBase), "stcbase");
  memset(base, 0x00, sizeof(STCBase));
  stcBaseInit(base); /* inits RLEAnchor, workQueue, socketSet, logContext */

  printf("STC Base inited\n");

  int returnCode = 0;
  int reasonCode = 0;
  // HttpServer *server = makeHttpServer(base, HTTP_PORT, &returnCode, &reasonCode);
  HttpServer *server = makeHttpServer2(base, NULL, HTTP_PORT, 0, &returnCode, &reasonCode);
  if (server != NULL) {

  } else {
    printf("Server not created, RC = %d, RSN = %d\n", returnCode, reasonCode);
    return 8; // with memory leak
  }

  installServices(server);

  printf("About start HTTP server on port %d\n", HTTP_PORT);
  mainHttpLoop(server); // Loop for HTTP server
  server = NULL;

  stcBaseTerm(base);
  safeFree31((char *)base, sizeof(STCBase));
  base = NULL;

  return 0;
}