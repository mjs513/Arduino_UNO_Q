K_MUTEX_DEFINE(my_mutex);

void thread1(void *p1, void *p2, void *p3)
{
    while (1) {
        k_mutex_lock(&my_mutex, K_FOREVER);
        printk("Thread 1 is accessing the shared resource\n");
        k_sleep(K_MSEC(1000));
        k_mutex_unlock(&my_mutex);
        k_sleep(K_MSEC(1000));
    }
}

void thread2(void *p1, void *p2, void *p3)
{
    while (1) {
        k_mutex_lock(&my_mutex, K_FOREVER);
        printk("Thread 2 is accessing the shared resource\n");
        k_sleep(K_MSEC(1000));
        k_mutex_unlock(&my_mutex);
        k_sleep(K_MSEC(1000));
    }
}

#define STACK_SIZE 1024
#define PRIORITY 7

K_THREAD_STACK_DEFINE(thread1_stack, STACK_SIZE);
struct k_thread thread1_data;
k_tid_t thread1_tid;

K_THREAD_STACK_DEFINE(thread2_stack, STACK_SIZE);
struct k_thread thread2_data;
k_tid_t thread2_tid;

void setup() {
  thread1_tid = k_thread_create(&thread1_data, thread1_stack, K_THREAD_STACK_SIZEOF(thread1_stack),
    thread1, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);

  thread2_tid = k_thread_create(&thread2_data, thread2_stack, K_THREAD_STACK_SIZEOF(thread2_stack),
    thread2, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);


}

void loop() {
  // put your main code here, to run repeatedly:
  k_msleep(1);
}