import platform
import cpuinfo
import subprocess
import os

def main():
    print "Information about the computer: "
    info = cpuinfo.get_cpu_info()
    print info['brand']
    print platform.platform()
    # os.call()
    my_env = os.environ.copy()
    my_env["PATH"] = "/usr/sbin:/sbin:" + my_env["PATH"]
    my_command = 'make'
    process = subprocess.Popen(my_command, env=my_env, shell=True)
    out = process.wait()


if __name__ == "__main__":
    main()

