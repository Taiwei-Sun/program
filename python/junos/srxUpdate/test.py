import os, sys, logging
from jnpr.junos import Device
from jnpr.junos.utils.sw import SW
from jnpr.junos.exception import ConnectError
import time

host = '172.18.1.98'
package1 = '/var/lib/tftpboot/junos-srxsme-15.1X49-D180.2-domestic.tgz'
package2 = '/var/lib/tftpboot/jfirmware-srxsme-15.1X49-D180.2-signed.tgz'
remote_path = '/var/tmp'
validate = True
logfile = './log/install.log'
user = "lab"
passwd = "lab123"

def update_progress(dev, report):
    # log the progress of the installing process
    logging.info(report)


def main():

    # initialize logging
    logging.basicConfig(filename=logfile, level=logging.INFO,format='%(asctime)s:%(name)s: %(message)s')
    logging.getLogger().name = host
    logging.getLogger().addHandler(logging.StreamHandler())
    logging.info('Information logged in {0}'.format(logfile))

    # verify package1 exists
    if not (os.path.isfile(package1)):
        msg = 'Software package1 does not exist: {0}. '.format(package1)
        logging.error(msg)
        sys.exit()

    dev = Device(host=host, user=user, passwd=passwd)
    try:
        dev.open()
    except ConnectError as err:
        logging.error('Cannot connect to device: {0}\n'.format(err))
        return

    # Create an instance of SW
    sw = SW(dev)

    try:
        logging.info('Starting the software upgrade process: {0}'.format(package1))
        ok = True
        #ok = sw.install(package=package1, remote_path=remote_path,progress=update_progress, validate=validate)

    except Exception as err:
        msg = 'Unable to install software, {0}'.format(err)
        logging.error(msg)
        ok = False

    if ok is True:
        logging.info('Software installation complete. Rebooting')
        rsp = sw.reboot()
        logging.info('Upgrade pending reboot cycle, please be patient.')
        logging.info(rsp)

		
		
		
    else:
        msg = 'Unable to install software, {0}'.format(ok)
        logging.error(msg)

		
		
	#dev=None
	#dev = Device(host=host, user=user, passwd=passwd)
    # End the NETCONF session and close the connection
    time.sleep(20)
    rebootOK = False
    while not rebootOK:
        try:
            print("try open")
            time.sleep(15)
            rebootOK = True
            dev.open()

        except ConnectError as err:
            print("Not open")
            rebootOK = False


#"--------------firmware--------------"
# verify package2 exists
    if not (os.path.isfile(package2)):
        msg = 'Software package2 does not exist: {0}. '.format(package2)
        logging.error(msg)
        sys.exit()

    

    # Create an instance of SW
    sw = SW(dev)

    try:
        logging.info('Starting the software upgrade process: {0}'.format(package2))
        #ok = True
        ok = sw.install(package=package2, remote_path=remote_path,progress=update_progress, validate=validate)
    except Exception as err:
        msg = 'Unable to install software, {0}'.format(err)
        logging.error(msg)
        ok = False

    if ok is True:
        logging.info('Software installation complete.')
        dev.cli("request system firmware upgrade pic pic-slot 0 fpc-slot 1")

    else:
        msg = 'Unable to install software, {0}'.format(ok)
        logging.error(msg)
    
    time.sleep(20)
    rsp = sw.reboot()
    print("last reboot")
	
	
    time.sleep(20)
    rebootOK = False
    while not rebootOK:
        try:
            print("try open")
            time.sleep(15)
            rebootOK = True
            dev.open()

        except ConnectError as err:
            print("Not open")
            rebootOK = False
	
    #dev.timeout = 600 
	#dev.rpc.request_snapshot(slice='alternate')
    #dev.cli("request system snapshot slice alternate")
    
    print "close device"
    dev.close()

if __name__ == "__main__":
    main()
