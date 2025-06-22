#! /bin/bash
### BEGIN INIT INFO
# Provides: excetpd
# Required-Start: $network $syslog $mysql
# Required-Stop: $network $syslog $mysql
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: A java exception report daemon.
### END INIT INFO

# Sustituya "proy1" por el nombre del ejecutable de su proyecto.
SERVNAME=aplicacion

start() {
    check_dependencies || exit 1

    PID=`pidof $SERVNAME`
    if [ $? -eq 0 ];
        then
        echo "$SERVNAME already started."
	    return 1
    fi

    echo "Starting $SERVNAME"
    runuser -l daemon_user -c "$SERVNAME &"

    if [$? -eq 0]; then
        echo $! > /var/run/$SERVNAME.pid
        logger -t $SERVNAME "Starting (PID $!)."
        return 0
    else
        logger -t $SERVNAME "Fail init"
        return 1
    fi
}

stop() {
    [ -f /var/run/$SERVNAME.pid] || {echo "FAILED: $SERVNAME is not running"; return 1;}

    PID=`pidof $SERVNAME`
    echo "Stopping $SERVNAME. (PID $PID)..."

    kill -TERM $PID
    for i in {1..5}; 
    do
        [ ! -d /proc/$PID] && break
        sleep 2
    done

    [ -d /proc/$PID ] && kill -KILL $PID
    rm -f /var/run/$SERVNAME.pid
	logger -t $SERVNAME "already stopped."
}

case "$1" in
    start)
        start
        ;;
    stop)
        stop
        ;;
    status)
	PID=`pidof $SERVNAME`
	if [ $? -eq 0 ]
	then
	    echo "$SERVNAME is running."
	else
	    echo "$SERVNAME is down."
	fi
        ;;
    restart)
        stop
        start
        ;;
    *)
        echo "Usage:  {start|stop|restart|status}"
        exit 1
        ;;
esac
exit $?
