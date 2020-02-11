import os, sys, logging
from jnpr.junos import Device
from jnpr.junos.utils.sw import SW
from jnpr.junos.exception import ConnectError

host = '172.18.18.98'
package = '/var/lib/tftpboot/jfirmware-srxsme-15.1X49-D180.2-signed.tgz'
#remote_path = '/var/tmp'
validate = True
logfile = '/var/log/junos-pyez-install.log'
junos_username="lab"
junos_password="lab123"

def update_progress(dev, report):
    # log the progress of the installing process
    logging.info(report)


def main():

    # initialize logging
    logging.basicConfig(filename=logfile, level=logging.INFO,
                        format='%(asctime)s:%(name)s: %(message)s')
    logging.getLogger().name = host
    logging.getLogger().addHandler(logging.StreamHandler())
    logging.info('Information logged in {0}'.format(logfile))

    # verify package exists
    if not (os.path.isfile(package)):
        msg = 'Software package does not exist: {0}. '.format(package)
        logging.error(msg)
        sys.exit()

    dev = Device(host=host,user=junos_username, passwd=junos_password)
    try:
        dev.open()
    except ConnectError as err:
        logging.error('Cannot connect to device: {0}\n'.format(err))
        return

    # Create an instance of SW
    sw = SW(dev)

    try:
        logging.info('Starting the software upgrade process: {0}' .format(package))
        #ok = sw.install(package=package,progress=update_progress)
    except Exception as err:
        msg = 'Unable to install software, {0}'.format(err)
        logging.error(msg)
        ok = False
    #UPGRADED SUCCESSFULLY
    logging.info("!!!!!!!!!!!!!")
    #logging.info(dev.cli("request system firmware upgrade pic pic-slot 0 fpc-slot 1"))
    print dev.cli("show system firmware")
    
    logging.info("!!!!!!!!!!!!!")

    if ok is True:
        logging.info('Software installation complete. Rebooting')
        #rsp = sw.reboot()
        logging.info('Upgrade pending reboot cycle, please be patient.')
        logging.info(rsp)
    else:
        msg = 'Unable to install software, {0}'.format(ok)
        logging.error(msg)

    # End the NETCONF session and close the connection
    dev.close()

if __name__ == "__main__":
    main()


