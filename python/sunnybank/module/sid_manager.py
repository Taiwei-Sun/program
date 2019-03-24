from datetime import datetime, timedelta
import uuid

from gevent.coros import Semaphore

from sunnybank.module.utils import (ThreadWorker, WorkerTask)


class SidManager(WorkerTask):

    class SidMember(object):

        def __init__(self, sid, user_name, role, keep_time):
            self.user_name = user_name
            self.role = role
            self.keep_time = keep_time
            self.expire = (datetime.now() + timedelta(seconds=int(keep_time)))
            self.sid = sid

        @property
        def is_expired(self):
            return datetime.now() > self.expire

    class SidWorker(ThreadWorker):
        def __init__(self):
            self.sid_mgr = None
            super(SidManager.SidWorker, self).__init__(60)

        def do_work(self):
            print 'Starting check sid time'
            for _s in self.sid_mgr.sid_dict.values():
                if _s.is_expired:
                    print 'sid: %s expired!' % _s.sid
                    self.sid_mgr.del_sid(_s.sid)

    def __init__(self, worker):
        self.sid_dict = None
        self.semaphore = None
        super(SidManager, self).__init__(worker)

    def generate_sid(self):
        return uuid.uuid4().hex.replace('-', '')

    def add_sid(self, user_name, role, keep_time):
        sid = self.generate_sid()
        with self.semaphore:
            self.sid_dict[sid] = SidManager.SidMember(sid, user_name,
                                                      role, keep_time)
            return sid

    def del_sid(self, sid):
        with self.semaphore:
            del self.sid_dict[sid]

    def extend_sid_time(self, sid):
        with self.semaphore:
            _s = self.sid_dict.pop(sid)
            _s.expire = (datetime.now() + timedelta(seconds=int(_s.keep_time)))
            self.sid_dict[sid] = _s

    def check_sid(self, sid):
        if sid not in self.sid_dict:
            return False

        return True

    @classmethod
    def _init_instance(cls):
        _worker = SidManager.SidWorker()
        _instance = cls(_worker)
        _instance.sid_dict = dict()
        _instance.semaphore = Semaphore()
        _instance.worker.sid_mgr = _instance
        return _instance
