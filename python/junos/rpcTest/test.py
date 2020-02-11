#!/usr/bin/env python3

from jnpr.junos import Device
from ENC_Client.module.jnpr_rpc.jnpr_interface_info import GetInterfaceInformation

dev=None

def dev_open(conn_info):
    print('DevOpen')
    try:
        dev = Device(**conn_info)
        dev.open()
        dev.timeout = 1800
    except RuntimeError as err:
        print(f"[{conn_info['host']}] Open failed: Runtime Error")
    except ConnectAuthError as err:
        print(f"[{conn_info['host']}] Open failed: Connect Auth Error")
    except ConnectError as err:
        print(f"[{conn_info['host']}] Open failed: Cannot connect to device")
    except Exception as err:
        print(f"[{conn_info['host']}] Device open failed")
        print(f"[{conn_info['host']}] Device open failed: ")
        


def dev_close():
    print('DevClose')
    try:
        if dev:
            dev.close()
    except:
        print('Device close failed')
        
        
        


try:
    eths_rpc = GetInterfaceInformation(self.dev).get() if self._config.config['interface_rpc'] else []
except:
    log.debug(f'[{self.dev.facts["hostname"]}] Get eths rpc error: {exception_message()}')
    raise Exception(f'Get eths rpc error')