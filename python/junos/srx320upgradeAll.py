import os, sys, logging
from jnpr.junos import Device
from jnpr.junos.utils.sw import SW
from jnpr.junos.exception import ConnectError

from lxml import etree
import xml.etree.ElementTree as ET
import time
import os

packageOS = '/var/lib/tftpboot/junos-srxsme-15.1X49-D180.2-domestic.tgz'
packageFirmware = '/var/lib/tftpboot/jfirmware-srxsme-15.1X49-D180.2-signed.tgz'
remote_path = '/var/tmp'
local_path='./log/'
validate = True
junos_username="lab"
junos_password="lab123"
rebootTimeout=600



def check_ping(hostname):
    response = os.system("ping -c 1 " + hostname)
    # and then check the response...
    if response == 0:
        pingstatus = "Network Active"
    else:
        pingstatus = "Network Error"

    return pingstatus



def update_progress(dev, report):
    # log the progress of the installing process
    logging.info(report)

def create_log(dev)

    #check serial number, logFile=this nember
    f = open(logFile, "a")
    f.write("Now the file has more content!")
    
    
    
    
    #show system alarm|no-more
    #show chassis alarm|no-more
    #show chassis environment|no-more
    #show chassis routing-engine|no-more
    #show chassis hardware|no-more
    #show chassis fpc|no-more
    #show chassis fpc pic-status|no-more   
    #show system license|no-more
    #show version                          
    #show system snapshot media internal |no-more
    #show modem wireless network cl-1/0/0|no-more
    #show system firmware|no-more 
    #show modem wireless profiles cl-1/0/0 slot 1

    
    
    
    
    f.close()


def update_process(type,host,package)
    
    # verify package exists
    if not (os.path.isfile(package)):
        msg = 'Software package does not exist: {0}. '.format(package)
        logging.error(msg)
        sys.exit()
    
    dev = Device(host=host,user=junos_username, passwd=junos_password)
    
    dev=None
    rebootTime=10
    rebootOK=False
    while not rebootOK:
        try:
            time.sleep(10)
            rebootOK=True
            dev.open()
        except Exception as err:
            logging.info('wait device time='+str(rebootTime)+"Sec  "+str(err))
            rebootTime+=10
            rebootOK=False

        if rebootTime>=rebootTimeout:
            logging.error('Cannot connect to device\n')
            return
    
    if type=='createLogFile':
        create_log(dev)
        dev.close()
        return
    
    
    # Create an instance of SW
    sw = SW(dev)
    
    try:
        logging.info('Starting the software upgrade process: {0}'.format(package))
        okOS = sw.install(package=package, remote_path=remote_path,progress=update_progress, validate=validate)
    except Exception as err:
        msg = 'Unable to install software, {0}'.format(err)
        logging.error(msg)
        okOS = False
    
    
    if type=="Firmware":
        #UPGRADED SUCCESSFULLY
        logging.info("!!!!!!!!!!!!!")
        logging.info(dev.cli("request system firmware upgrade pic pic-slot 0 fpc-slot 1"))
        
        
        okFirmware=False
        waitFirmwareTime=0
        
        while not okFirmware and okOS:
            xmlStr=etree.tostring(dev.rpc.get_system_firmware_information(), encoding='unicode')
            xmlRoot = ET.fromstring(xmlStr)
            for firmware_status in xmlRoot.iter('firmware-status'):
                #UPGRADED SUCCESSFULLY
                if firmware_status.text=='UPGRADED SUCCESSFULLY':okFirmware=True
                if firmware_status.text!='OK':print firmware_status.text
            print waitFirmwareTime
            time.sleep(1)
            waitFirmwareTime=waitFirmwareTime+1
            #print okFirmware
    else if type=="OS":
        okFirmware=True
    
    
    
    
    if okOS is True and okFirmware is True:
        logging.info('Software installation complete. Rebooting')
        rsp = sw.reboot()
        logging.info('Upgrade pending reboot cycle, please be patient.')
        logging.info(rsp)
    else:
        msg = 'Unable to install software, {0}'.format(ok)
        logging.error(msg)
    
    # End the NETCONF session and close the connection
    dev.close()

def main():

    bug_log=local_path+'junos-pyez-install.log'
    # initialize logging
    logging.basicConfig(filename=bug_log, level=logging.INFO,format='%(asctime)s:%(name)s: %(message)s')
    logging.getLogger().name = srx320-upgrade
    #logging.getLogger().addHandler(logging.StreamHandler())#when run this python, terminals can show log
    logging.info('Information logged in {0}'.format(bug_log))

    try:  
        os.mkdir(local_path)
    except OSError:  
        print ("Creation of the directory %s failed" % path)
    else:  
        print ("Successfully created the directory %s " % path)

    installIP='172.18.1.98'
    update_process("OS",installIP,packageOS)

    print '!!!!!!!!!!!!!!!'
    update_process("Firmware",installIP,packageFirmware)





if __name__ == "__main__":
    main()


