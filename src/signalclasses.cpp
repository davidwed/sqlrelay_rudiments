// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/signalclasses.h>
#include <rudiments/error.h>
#include <rudiments/bytestring.h>
#if !defined(RUDIMENTS_HAVE_SIGACTION)
	#include <rudiments/linkedlist.h>
#endif
#if defined(RUDIMENTS_HAVE_GENERATECONSOLECTRLEVENT)
	#include <rudiments/process.h>
	#include <rudiments/sys.h>
#endif

#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif

#include <stdio.h>

class signalsetprivate {
	friend class signalset;
	friend class signalmanager;
	friend class signalhandler;
	private:
		#if defined(RUDIMENTS_HAVE_SIGACTION)
			sigset_t	_sigset;
		#else
			linkedlist< int32_t >	_siglist;
		#endif
};

// signalset methods
signalset::signalset() {
	pvt=new signalsetprivate;
	removeAllSignals();
}

signalset::~signalset() {
	#if !defined(RUDIMENTS_HAVE_SIGACTION)
		pvt->_siglist.clear();
	#endif
	delete pvt;
}

bool signalset::addSignal(int32_t signum) {
	#if defined(RUDIMENTS_HAVE_SIGACTION)
		return !sigaddset(&pvt->_sigset,signum);
	#else
		if (!pvt->_siglist.find(signum)) {
			pvt->_siglist.append(signum);
		}
		return true;
	#endif
}

bool signalset::addAllSignals() {
	#if defined(RUDIMENTS_HAVE_SIGACTION)
		return !sigfillset(&pvt->_sigset);
	#else
		#ifdef SIGHUP
			addSignal(SIGHUP);
		#endif
		#ifdef SIGINT
			addSignal(SIGINT);
		#endif
		#ifdef SIGQUIT
			addSignal(SIGQUIT);
		#endif
		#ifdef SIGILL
			addSignal(SIGILL);
		#endif
		#ifdef SIGTRAP
			addSignal(SIGTRAP);
		#endif
		#ifdef SIGABRT
			addSignal(SIGABRT);
		#endif
		#ifdef SIGIOT
			addSignal(SIGIOT);
		#endif
		#ifdef SIGBUS
			addSignal(SIGBUS);
		#endif
		#ifdef SIGFPE
			addSignal(SIGFPE);
		#endif
		#ifdef SIGUSR1
			addSignal(SIGUSR1);
		#endif
		#ifdef SIGSEGV
			addSignal(SIGSEGV);
		#endif
		#ifdef SIGUSR2
			addSignal(SIGUSR2);
		#endif
		#ifdef SIGPIPE
			addSignal(SIGPIPE);
		#endif
		#ifdef SIGALRM
			addSignal(SIGALRM);
		#endif
		#ifdef SIGTERM
			addSignal(SIGTERM);
		#endif
		#ifdef SIGSTKFLT
			addSignal(SIGSTKFLT);
		#endif
		#ifdef SIGCHLD
			addSignal(SIGCHLD);
		#endif
		#ifdef SIGCONT
			addSignal(SIGCONT);
		#endif
		#ifdef SIGSTOP
			addSignal(SIGSTOP);
		#endif
		#ifdef SIGTSTP
			addSignal(SIGTSTP);
		#endif
		#ifdef SIGTTIN
			addSignal(SIGTTIN);
		#endif
		#ifdef SIGTTOU
			addSignal(SIGTTOU);
		#endif
		#ifdef SIGURG
			addSignal(SIGURG);
		#endif
		#ifdef SIGXCPU
			addSignal(SIGXCPU);
		#endif
		#ifdef SIGXFSZ
			addSignal(SIGXFSZ);
		#endif
		#ifdef SIGVTALRM
			addSignal(SIGVTALRM);
		#endif
		#ifdef SIGPROF
			addSignal(SIGPROF);
		#endif
		#ifdef SIGWINCH
			addSignal(SIGWINCH);
		#endif
		#ifdef SIGIO
			addSignal(SIGIO);
		#endif
		#ifdef SIGPOLL
			addSignal(SIGPOLL);
		#endif
		#ifdef SIGPWR
			addSignal(SIGPWR);
		#endif
		#ifdef SIGUNUSED
			addSignal(SIGUNUSED);
		#endif
		#ifdef SIGEMT
			addSignal(SIGEMT);
		#endif
		#ifdef SIGSYS
			addSignal(SIGSYS);
		#endif
		#ifdef SIGWAITING
			addSignal(SIGWAITING);
		#endif
		#ifdef SIGLWP
			addSignal(SIGLWP);
		#endif
		#ifdef SIGFREEZE
			addSignal(SIGFREEZE);
		#endif
		#ifdef SIGTHAW
			addSignal(SIGTHAW);
		#endif
		#ifdef SIGCANCEL
			addSignal(SIGCANCEL);
		#endif
		#ifdef SIGLOST
			addSignal(SIGLOST);
		#endif
		#ifdef _SIGRTMIN
			addSignal(_SIGRTMIN);
		#endif
		#ifdef _SIGRTMAX
			addSignal(_SIGRTMAX);
		#endif
		#ifdef SIGRTMIN
			addSignal(SIGRTMIN);
		#endif
		#ifdef SIGRTMAX
			addSignal(SIGRTMAX);
		#endif
		return true;
	#endif
}

bool signalset::removeSignal(int32_t signum) {
	#if defined(RUDIMENTS_HAVE_SIGACTION)
		return !sigdelset(&pvt->_sigset,signum);
	#else
		pvt->_siglist.remove(signum);
		return true;
	#endif
}

bool signalset::removeAllSignals() {
	#if defined(RUDIMENTS_HAVE_SIGACTION)
		return !sigemptyset(&pvt->_sigset);
	#else
		pvt->_siglist.clear();
		return true;
	#endif
}

int32_t signalset::signalIsInSet(int32_t signum) const {
	#if defined(RUDIMENTS_HAVE_SIGACTION)
		return sigismember(&pvt->_sigset,signum);
	#else
		return (pvt->_siglist.find(signum)!=NULL);
	#endif
}



// signalmanager methods
bool signalmanager::sendSignal(pid_t processid, int32_t signum) {
	#ifdef RUDIMENTS_HAVE_KILL
		int32_t	result;
		do {
			result=kill(processid,signum);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_GENERATECONSOLECTRLEVENT)

		// For now, bail unless the signal is SIGINT or SIGTERM
		if (signum!=SIGINT && signum!=SIGTERM) {
			return false;
		}

		// decide what access rights we need
		DWORD	accessrights=PROCESS_TERMINATE;
		if (signum==SIGINT) {
			accessrights=PROCESS_CREATE_THREAD|
					PROCESS_QUERY_INFORMATION|
					PROCESS_VM_OPERATION|
					PROCESS_VM_WRITE|
					PROCESS_VM_READ;
		}

		// open the target process
		HANDLE	targetprocess=OpenProcess(accessrights,FALSE,processid);
		if (!targetprocess) {
			return false;
		}

		// for SIGTERM we just need to call TerminateProcess
		if (signum==SIGTERM) {
			bool	result=(TerminateProcess(targetprocess,1)!=0);
			CloseHandle(targetprocess);
			return result;
		}

		// For SIGINT, it gets a lot crazier...

		// Yes, the ridiculousness below is the only "reasonable"
		// way to do this...

		// Ideally we'd just run
		// GenerateConsoleCtrlEvent(CTRL_C_EVENT,processid) but that
		// only works if the calling process is in the same process
		// group as processid (ie. a parent or child of processid).
		// 
		// So, we need to somehow coerce a process or thread in the
		// target process group to run it for us.  We can create a new
		// thread in the target process using CreateRemoteThread.
		// 
		// Ideally we'd just tell it to run
		// GenerateConsoleCtrlEvent(CTRL_C_EVENT,0) but
		// CreateRemoteThread only allows you to pass one argument
		// to the function that it runs and we need to pass two.
		//
		// The only "obvious" way to do this is to do define a chunk
		// of memory containing the machine code for a function that
		// runs GenerateConsoleCtrlEvent(CTRL_C_EVENT,0) and copy it
		// over to the target process.
 		//
		// Then we can create a thread over there and aim the thread at
		// our function.

		// this only works on x86 and x64, so bail right away if
		// this isn't one of those platforms
		char	*arch=sys::getOperatingSystemArchitecture();
		if (!charstring::compare(arch,"Unknown")) {
			return false;
		}

		// Get the address of the appropriate function in kernel32.dll.
		// For SIGINT, use GenerateControlCtrlEvent.
		// For SIGTERM use ExitProcess.
		// kernel32.dll is loaded at the same address for all programs,
		// so the address of this function ought to be the same in
		// the target process as it is here.
		HMODULE	kernel32=GetModuleHandle("Kernel32");
		if (!kernel32) {
			return false;
		}
		FARPROC	funcaddr=GetProcAddress(kernel32,
						"GenerateConsoleCtrlEvent");
		if (!funcaddr) {
			return false;
		}

		// Define a chunk of memory containing the machine code for
		// a function that runs GenerateConsoleCtrlEvent with two
		// parameters (with values of 0).  Eventually this code will
		// be run in the target process...
		//
		// helpful site:
		// https://defuse.ca/online-x86-assembler.htm

		const unsigned char	*updatedmachinecode=NULL;
		size_t			machinecodesize=0;

		// FIXME: use better method of detecting x86 vs. x64
		#ifdef _USE_32BIT_TIME_T

			// for x86:
			const unsigned char	machinecode32[]={
				// load second parameter (0)
				0x6A,			// push 
				0x0,			// 0
				// load first parameter (0)
				0x6A,			// push 
				0x0,			// 0
				// load the absolute address of the function to
				// call (for now use 0, we'll overwrite this in
				// a minute)
				0xB8,			// mov eax
				0x0,0x0,0x0,0x0,	// 0
				// call the function
				0xFF,0xD0,		// call eax
				// return 1
				0xB8,			// mov eax
				0x1,0x0,0x0,0x0,	// 1
				// return
				0xC3			// ret
			};
			size_t		machinecode32size=sizeof(machinecode32);

			// copy the code into a buffer and
			// replace the call address
			unsigned char		*updatedmachinecode32=
				(unsigned char *)bytestring::duplicate(
							machinecode32,
							machinecode32size);
			uint32_t	*addr=
				(uint32_t *)(updatedmachinecode32+5);
			*addr=(uint32_t)funcaddr;
			updatedmachinecode=updatedmachinecode32;
			machinecodesize=machinecode32size;

		#else

			// for x64:
			const unsigned char	machinecode64[]={
				// allocate shadow space of 32 bytes on the
				// stack and align it to 16 bytes
				0x48,0x83,0xEC,		// sub rsp
				0x28,			// 0x28
				// load second parameter (0)
				0x48,0xC7,0xC2,		// mov rdx
				0x00,0x00,0x00,0x00,	// 0
				// load first parameter (0)
				0x48,0xC7,0xC1,		// mov rcx
				0x00,0x00,0x00,0x00,	// 0
				// load the absolute address of the function to
				// call (for now use 0, we'll overwrite this in
				// a minute)
				0x49,0xBA,		// movabs r10
				0x00,0x00,0x00,0x00,	// 0
				0x00,0x00,0x00,0x00,	// 0 (64-bit)
				// call the function
				0x41,0xFF,0xD2,		// call r10
				// return 1
				0x48,0xC7,0xC0,		// mov rax
				0x01,0x00,0x00,0x00,	// 1
				// deallocate the shadow space
				// and align the stack to 16 bytes
				0x48,0x83,0xC4,		// add rsp
				0x28,			// 0x28
				// return
				0xC3			// retq
			};
			size_t		machinecode64size=sizeof(machinecode64);

			// copy the code into a buffer and
			// replace the call address
			unsigned char		*updatedmachinecode64=
				(unsigned char *)bytestring::duplicate(
							machinecode64,
							machinecode64size);
			uint64_t	*addr=
				(uint64_t *)(updatedmachinecode64+20);
			*addr=(uint64_t)funcaddr;
			updatedmachinecode=updatedmachinecode64;
			machinecodesize=machinecode64size;
		#endif

		// allocate memory in the target process to copy the
		// machine code into
		void	*codetorun=VirtualAllocEx(targetprocess,
						NULL,machinecodesize,
						MEM_COMMIT,
						PAGE_EXECUTE_READWRITE);
		if (!codetorun) {
			CloseHandle(targetprocess);
			return false;
		}

		// copy the machine code over to the target process
		if (!WriteProcessMemory(targetprocess,
						codetorun,
						updatedmachinecode,
						machinecodesize,
						NULL)) {
			CloseHandle(targetprocess);
			return false;
		}

		// create a thread in the target process and aim it at
		// the machine code we passed over there
		HANDLE	otherthread=
			CreateRemoteThread(targetprocess,
					NULL,0,
					(LPTHREAD_START_ROUTINE)codetorun,
					NULL,0,NULL);
		if (!otherthread) {
			CloseHandle(targetprocess);
			return false;
		}

		// wait for the thread to finish running
		WaitForSingleObject(otherthread,INFINITE);

		// clean up
		VirtualFreeEx(targetprocess,codetorun,0,MEM_RELEASE);
		CloseHandle(otherthread);
		CloseHandle(targetprocess);
		
		return true;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool signalmanager::raiseSignal(int32_t signum) {
	#if defined(RUDIMENTS_HAVE_GENERATECONSOLECTRLEVENT)
		if (signum==SIGINT || signum==SIGTERM) {
			if (GenerateConsoleCtrlEvent(CTRL_C_EVENT,0)) {
				return true;
			}
		}
		return false;
	#elif defined(RUDIMENTS_HAVE_RAISE)
		int32_t	result;
		do {
			result=raise(signum);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

uint32_t signalmanager::alarm(uint32_t seconds) {
	#ifdef RUDIMENTS_HAVE_ALARM
		return ::alarm(seconds);
	#else
		error::setErrorNumber(ENOSYS);
		return 0;
	#endif
}

bool signalmanager::ignoreSignals(const signalset *sset) {
	#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER) || \
		defined(RUDIMENTS_HAVE_SETUNHANDLEDEXCEPTIONFILTER)
		bool	result=true;
		for (linkedlistnode< int32_t > *node=
				sset->pvt->_siglist.getFirst();
				node; node=node->getNext()) {

			switch (node->getValue()) {
				case SIGINT:
				case SIGTERM:
			#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER)
					result=(result &&
						SetConsoleCtrlHandler(
							(PHANDLER_ROUTINE)NULL,
							TRUE)==TRUE);
			#endif
				case SIGABRT:
				case SIGFPE:
				case SIGILL:
				case SIGSEGV:
			#if defined(RUDIMENTS_HAVE_SETUNHANDLEDEXCEPTIONFILTER)
					SetUnhandledExceptionFilter(NULL);
			#endif
			}
		}
		return result;
	#elif defined(RUDIMENTS_HAVE_SIGACTION)
		int32_t	result;
		do {
			#ifdef RUDIMENTS_HAVE_PTHREAD_SIGMASK
				result=pthread_sigmask(SIG_SETMASK,
						&sset->pvt->_sigset,NULL);
			#else
				result=sigprocmask(SIG_SETMASK,
						&sset->pvt->_sigset,NULL);
			#endif
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_SIGNAL)
		bool	result=true;
		for (linkedlistnode< int32_t > *node=
				sset->pvt->_siglist.getFirst();
				node; node=node->getNext()) {
			result=(result &&
				(signal(node->getValue(),SIG_IGN)!=SIG_ERR));
		}
		return result;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool signalmanager::waitForSignals(const signalset *sset) {
	#ifdef RUDIMENTS_HAVE_KILL
		return (sigsuspend(&sset->pvt->_sigset)==-1);
	#else
		// see sendSignal above...
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool signalmanager::examineBlockedSignals(signalset *sset) {
	#ifdef RUDIMENTS_HAVE_KILL
		int32_t	result;
		do {
			result=sigpending(&sset->pvt->_sigset);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		// see sendSignal above...
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}


class signalhandlerprivate {
	friend class signalhandler;
	private:
		const signalset	*_sset;
		int32_t		_flags;
		void		(*_handler)(int32_t);
		#if defined(RUDIMENTS_HAVE_SIGACTION)
			struct sigaction	_handlerstruct;
		#endif
		#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER)
			static signalhandlerprivate	*_sigsegvinstance;
			static LONG	_sigsegvFilter(
						struct _EXCEPTION_POINTERS *ei);
		#endif
};

// signalhandler methods
signalhandler::signalhandler() {
	pvt=new signalhandlerprivate;
	pvt->_sset=NULL;
	pvt->_flags=0;
	pvt->_handler=NULL;
}

signalhandler::~signalhandler() {
	delete pvt;
}

void signalhandler::setHandler(void (*handler)(int32_t)) {
	pvt->_handler=handler;
}

void (*signalhandler::getHandler())(int32_t) {
	return pvt->_handler;
}

void signalhandler::removeAllFlags() {
	pvt->_flags=0;
}

void signalhandler::addFlag(int32_t flag) {
	pvt->_flags|=flag;
}

void signalhandler::removeFlag(int32_t flag) {
	pvt->_flags&=(~flag);
}

int32_t signalhandler::getFlags() const {
	return pvt->_flags;
}

void signalhandler::setMask(const signalset *sset) {
	pvt->_sset=sset;
}

const signalset *signalhandler::getMask() const {
	return pvt->_sset;
}

bool signalhandler::handleSignal(int32_t signum) {
	return handleSignal(signum,NULL);
}

#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER)
signalhandlerprivate	*signalhandlerprivate::_sigsegvinstance=NULL;

LONG signalhandlerprivate::_sigsegvFilter(struct _EXCEPTION_POINTERS *ei) {
	_sigsegvinstance->_handler(SIGSEGV);
	return EXCEPTION_CONTINUE_EXECUTION;
}
#endif

bool signalhandler::handleSignal(int32_t signum, signalhandler *oldhandler) {
	#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER) || \
		defined(RUDIMENTS_HAVE_SETUNHANDLEDEXCEPTIONFILTER)
		if (oldhandler) {
			// FIXME: set this somehow
			oldhandler->pvt->_handler=NULL;
		}
		switch (signum) {
			case SIGINT:
			case SIGTERM:
			#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER)
				return SetConsoleCtrlHandler(
					(PHANDLER_ROUTINE)pvt->_handler,
					TRUE)==TRUE;
			#endif
			case SIGABRT:
			case SIGFPE:
			case SIGILL:
			case SIGSEGV:
			#if defined(RUDIMENTS_HAVE_SETUNHANDLEDEXCEPTIONFILTER)
				{
					pvt->_sigsegvinstance=this->pvt;
					SetUnhandledExceptionFilter(
						(LPTOP_LEVEL_EXCEPTION_FILTER)
						pvt->_sigsegvFilter);
				}
				return true;
			#endif
		}
		error::setErrorNumber(ENOSYS);
		return false;
	#elif defined(RUDIMENTS_HAVE_SIGACTION)
		struct sigaction	oldaction;
		int32_t			result;
		if (pvt->_sset) {
			bytestring::copy(&pvt->_handlerstruct.sa_mask,
						&pvt->_sset->pvt->_sigset,
						sizeof(sigset_t));
		} else {
			sigemptyset(&pvt->_handlerstruct.sa_mask);
		}
		pvt->_handlerstruct.sa_flags=pvt->_flags;
		#ifdef RUDIMENTS_SIGNAL_HANDLER_INT
			pvt->_handlerstruct.sa_handler=
					(void(*)(int))pvt->_handler;
		#else
			pvt->_handlerstruct.sa_handler=
					(void(*)(void))pvt->_handler;
		#endif
		do {
			result=sigaction(signum,&pvt->_handlerstruct,
								&oldaction);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		if (oldhandler) {
			oldhandler->pvt->_handlerstruct=oldaction;
		}
		return !result;
	#elif defined(RUDIMENTS_HAVE_SIGNAL)
		void (*prev)(int)=signal(signum,pvt->_handler);
		if (oldhandler) {
			oldhandler->pvt->_handler=prev;
		}
		return (prev!=SIG_ERR);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool signalhandler::isSignalHandlerIntUsed() {
	#ifdef RUDIMENTS_SIGNAL_HANDLER_INT
		return true;
	#else
		return false;
	#endif
}
