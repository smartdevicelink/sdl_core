#!/usr/bin/env bash
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

apt-get install -y  distcc distccmon-gnome
echo "distcc g++ \$@" > /usr/local/bin/distcxx
chmod +x /usr/local/bin/distcxx

function ip_list() {
	ifconfig | sed -En 's/127.0.0.1//;s/.*inet (addr:)?(([0-9]*\.){3}[0-9]*).*/\2/p'
}

ip_addr=$(ip_list | grep 172| head -n 1)

DISTCC_CONFIG=/etc/default/distcc
sed -i 's/STARTDISTCC="false"/STARTDISTCC="true"/g' $DISTCC_CONFIG
sed -i -e 's/ALLOWEDNETS=.*/ALLOWEDNETS="172.16.0.0\/12"/g' $DISTCC_CONFIG
sed -i 's/LISTENER=.*/LISTENER="'$ip_addr'"/g' $DISTCC_CONFIG
sed -i 's/NICE=.*/NICE="19"/g' $DISTCC_CONFIG
sed -i 's/JOBS=.*/JOBS="4"/g' $DISTCC_CONFIG

SYTEMD=$(systemctl --version)
if [ -n "$SYTEMD" ]; then
	echo "Add distcc to autostart with systemd"
	systemctl start distcc
	systemctl enable distcc
	systemctl status distcc
else
	echo "Add distcc to autostart with service"
 	#correct previous set-up:
 	service stop distcc
 	rm -f /etc/init/distcc.conf
	update-rc.d distcc defaults
	service distcc start
 	service distcc status
fi

ifconfig
uname -a
lsb_release -a
g++ --version
