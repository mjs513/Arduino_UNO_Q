struct k_event my_event;
#define STACK_SIZE 1024
#define PRIORITY 7

K_THREAD_STACK_DEFINE(thread1_stack, STACK_SIZE);
struct k_thread thread1_data;
k_tid_t thread1_tid;


void thread1(void *p1, void *p2, void *p3) {
  while (1) {
    printk("%lu: Thread\n", (uint32_t)millis());
    k_sleep(K_MSEC(1000));
    k_event_set(&my_event, 0x001);
    k_sleep(K_MSEC(1000));
  }
}


void setup() {
  k_event_init(&my_event);
  thread1_tid = k_thread_create(&thread1_data, thread1_stack, K_THREAD_STACK_SIZEOF(thread1_stack),
                                thread1, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
}

void loop() {
  uint32_t events;
  events = k_event_wait(&my_event, 0xFFF, true, K_MSEC(2000));
  if (events == 0) {
    printk("Timeout");
  } else {
    printk("l%u:loop\n", (uint32_t)millis());
  }
}