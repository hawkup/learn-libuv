#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <uv.h>

#define FIB_UNTIL 25
uv_loop_t *loop;

long fib_(long t) {
    if (t == 0 || t == 1)
        return 1;
    else
        return fib_(t-1) + fib_(t-2);
}

void fib(uv_work_t *req) {
    int n = *(int *) req->data;
    if (random() % 2)
        sleep(1);
    else
        sleep(3);
    long fib = fib_(n);
    fprintf(stderr, "%dth fibonacci is %lu\n", n, fib);
}

void after_fib(uv_work_t *req, int status) {
    fprintf(stderr, "Done calculating %dth fibonacci\n", *(int *) req->data);
}

int main() {
    loop = uv_default_loop();

    int data[FIB_UNTIL];

    // Work request type
    uv_work_t req[FIB_UNTIL];
    int i;
    for (i = 0; i < FIB_UNTIL; i++) {
        data[i] = i;
        req[i].data = (void *) &data[i];

        // uv_loop_t* loop, 
        // uv_work_t* req,
        // uv_work_cb work_cb,
        // uv_after_work_cb after_work_cb
        // Initializes a work request which will run the given work_cb in athread from the threadpool.
        // Once work_cb is completed, after_work_cb will be called on the loop thread.
        uv_queue_work(loop, &req[i], fib, after_fib);
    }

    return uv_run(loop, UV_RUN_DEFAULT);
}