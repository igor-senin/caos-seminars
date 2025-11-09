CFLAGS := -I. -I.. -I../util

dup:
	gcc $(CFLAGS) dup.c -o dup
pipes/child_waiting:
	gcc $(CFLAGS) pipes/child_waiting.c -o pipes/child_waiting
pipes/fork_pipe:
	gcc $(CFLAGS) pipes/fork_pipe.c -o pipes/fork_pipe
pipes/pipe:
	gcc $(CFLAGS) pipes/pipe.c -o pipes/pipe
pipes/pipe_buf:
	gcc $(CFLAGS) pipes/pipe_buf.c -o pipes/pipe_buf
pipes/pipe_fork_dup:
	gcc $(CFLAGS) pipes/pipe_fork_dup.c -o pipes/pipe_fork_dup

clean:
	rm dup pipes/child_waiting pipes/fork_pipe pipes/pipe pipes/pipe_buf pipes/pipe_fork_dup 2>/dev/null || true
