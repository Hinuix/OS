#include <xinu.h>
#include <future.h>

future_t* future_alloc(future_mode_t mode, uint size, uint nelems) {
	intmask mask;
	mask = disable();

	struct future_t* f = (future_t*) getmem(sizeof(future_t));
	f->size = size;
	f->data = getmem(size);
	f->state = FUTURE_EMPTY;
	f->mode = mode;

	restore(mask);
	return f;


}

syscall future_free(future_t* f) {
	freemem(f->data, f->size);
	kill(f->pid);
	freemem((char*) f, sizeof(future_t));
	return OK;
}

syscall future_get(future_t* f, void* out) {

	intmask mask;
	mask = disable();

	if (f->state == FUTURE_WAITING) { 
		restore(mask); 
		return SYSERR; 
	}


	else if (f->state == FUTURE_EMPTY) { 
		f->state = FUTURE_WAITING;
		f->pid = currpid;
		suspend(currpid);
	}

	
	memcpy(out, f->data, f->size);
	memcpy(f->data, NULL, f->size);
	f->state = FUTURE_EMPTY;
	restore(mask);
	return OK;
		

}

syscall future_set(future_t* f, void* in) {

	intmask mask;
	mask = disable();

	if (f->state == FUTURE_READY) { 
		restore(mask);
		return SYSERR; 
	}

	else if (f->state == FUTURE_EMPTY) { 
		memcpy(f->data, in, f->size);
		f->state = FUTURE_READY;
		restore(mask);
		return OK;
	}

	else 
	{

		memcpy(f->data, in, f->size);
		f->state = FUTURE_READY;
		resume(f->pid);
		f->pid = NULL;
		restore(mask);
		return OK;
	}
}

