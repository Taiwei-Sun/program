import gevent


class ThreadWorker(object):
    '''
    C{ThreadWorker} is a threaded worker to run a given work defined in
    L{do_work} method.
    '''
    def __init__(self, time_interval=60, init_delay=0):
        '''
        Constructor
        @param time_interval: time interval (in seconds) between doing work
        @type time_interval: C{bool}
        @param init_delay: initial delay (in seconds) before doing first work
        @type init_delay: C{bool}
        '''
        self.time_interval = time_interval
        self.init_delay = init_delay
        self.__worker_thread_event = gevent.event.Event()
        self.__worker_thread = None
        self.stopped = False

    def start(self):
        '''
        Start worker thread.
        '''
        print("Starting  %s ThreadWorker" % self.__class__.__name__)
        self.stopped = False
        self.__worker_thread = gevent.spawn(self.__loop)

    def stop(self):
        '''
        Stop worker thread
        '''
        print("Stopping %s ThreadWorker" % self.__class__.__name__)
        self.stopped = True
        self.wakeup()
        self.__worker_thread.join()

    def wakeup(self):
        '''
        Wake up worker thread to do work once.
        '''
        self.__worker_thread_event.set()

    def __loop(self):
        print("ThreadWorker %s entered" % self.__class__.__name__)
        self.__worker_thread_event.wait(self.init_delay)
        while not self.stopped:
            try:
                self.do_work()
            except Exception as e:
                # This is normal since we don't know what kind of exception
                # do_work() will riase.
                # LOG.debug("do_work() failed")
                # LOG.debug(e)
                print e
            self.__worker_thread_event.clear()
            self.__worker_thread_event.wait(self.time_interval)
        # LOG.debug("ThreadWorker %s exiting" % self.__class__.__name__)

    def do_work(self):
        '''
        The actual work of the work thread.
        Derived class should override this function.
        '''


class WorkerTask(object):
    __manager = None

    def __init__(self, worker):
        self.worker = worker

    @classmethod
    def _init_instance(cls):
        raise NotImplementedError()

    @classmethod
    def get_instance(cls):
        if not cls.__manager:
            cls.__manager = cls._init_instance()
        return cls.__manager

    def wakeup_worker(self):
        self.worker.wakeup()

    def resurrect(self):
        self.worker.start()
