import os
import signal
import subprocess

# Kill master and worker processes

pids = subprocess.Popen(['ps', '-A'], stdout=subprocess.PIPE)
out, err = pids.communicate()

for line in out.splitlines():
    if 'word_finder' in str(line):
        pid = int(line.split(None, 1)[0])
        print("kill pid: " + str(pid))
        os.kill(pid, signal.SIGKILL)