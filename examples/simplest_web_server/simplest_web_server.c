// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

static const char *s_http_port = "8080";
static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  mg_mgr_init(&mgr, NULL);
  // printf("Starting web server on port %s\n", s_http_port);
  // nc = mg_bind(&mgr, s_http_port, ev_handler);
  // if (nc == NULL) {
  //   printf("Failed to create listener\n");
  //   return 1;
  // }

struct mg_bind_opts opts;
memset(&opts, 0, sizeof(opts));
const char *err = NULL;
opts.error_string = &err;
printf("Starting web server on port %s\n", s_http_port);
nc = mg_bind_opt(&mgr, s_http_port, ev_handler, opts);
if (nc == NULL) {
  fprintf(stderr, "mg_bind(%s) failed: %s\n", s_http_port, err);
  return 1;
}

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";  // Serve current directory
  s_http_server_opts.enable_directory_listing = "yes";

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
