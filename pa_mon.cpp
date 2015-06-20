#include <pulse/pulseaudio.h>
#include <pulse/mainloop.h>
#include <pulse/rtclock.h>

#include <pulse/error.h>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include <thread>

static void io_event_callback(pa_mainloop_api*a, pa_io_event *e, int fd, pa_io_event_flags_t f, void *userdata) {
  std::cout << "IO event" << std::endl;
}

static void context_state_callback(pa_context* context, void *userdata) {
  switch (pa_context_get_state(context)) {
  case PA_CONTEXT_UNCONNECTED:
    std::cout << "Context unconnected" << std::endl;
    break;
  case PA_CONTEXT_CONNECTING:
    std::cout << "Context connecting" << std::endl;
    break;
  case PA_CONTEXT_AUTHORIZING:
    std::cout << "Context authorizing" << std::endl;
    break;
  case PA_CONTEXT_SETTING_NAME:
    std::cout << "Context setting name" << std::endl;
    break;
  case PA_CONTEXT_READY:
    std::cout << "Context ready" << std::endl;
    break;
  case PA_CONTEXT_FAILED:
    std::cout << "Context failed" << std::endl;
    break;
  case PA_CONTEXT_TERMINATED:
    std::cout << "Context terminated" << std::endl;
    break;
  default:
    std::cout << "Something went wrong, invalid context code recieved" << std::endl;
  };
}

void start_mainloop(pa_mainloop* pulse, int* ret) {
  pa_mainloop_run(pulse, ret);
}

int main() {
  int32_t ret = 1;
  pa_mainloop* pulse = pa_mainloop_new();
  pa_mainloop_api* mainloop_api = pa_mainloop_get_api(pulse);

  pa_io_event* io_event = NULL;

  pa_sample_spec sample_spec;

  sample_spec.format = PA_SAMPLE_S16NE;
  sample_spec.channels = 2;
  sample_spec.rate = 44100;

  /*
  simple = pa_simple_new(NULL,
                         "RF Pulse lightshow monitor",
                         PA_STREAM_PLAYBACK,
                         NULL,
                         "Music",
                         &sample_spec,
                         NULL,
                         NULL,
                         NULL);
  */

  pa_context* context = pa_context_new(mainloop_api, "Rohde");
  pa_context_set_state_callback(context, context_state_callback, NULL);
  pa_context_connect(context, NULL, PA_CONTEXT_NOFLAGS , NULL);

  //io_event = mainloop_api->io_new(mainloop_api, 0, PA_IO_EVENT_OUTPUT, io_event_callback, NULL);
  std::thread t1 = std::thread(start_mainloop, pulse, &ret);

  std::cout << "Press enter to exit" << std::endl;
  std::string hell;
  std::getline(std::cin, hell);
  std::cout << hell << std::endl;

  pa_mainloop_quit(pulse, 0);
  pa_context_disconnect(context);

  t1.join();

  //  mainloop_api->io_free(io_event);
  pa_context_unref(context);
  pa_signal_done();
  pa_mainloop_free(pulse);
  return 0;
}
