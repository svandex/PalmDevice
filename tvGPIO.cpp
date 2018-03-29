#include "tvGPIO.h"

std::string gpioPrompt{"[GPIO]:"};
#define gOut std::cout<<gpioPrompt

int tvGPIO::numOfInstance = 0;

tvGPIO::tvGPIO()
{
	if (numOfInstance++ > 1) {
		gOut << "Only One Instance should be exist.";
		return;
	}
	gpioInitialise();
	gpioSetMode(TC_DC, PI_OUTPUT);
	gpioWrite(TC_DC, PI_HIGH);
        //Permisson change to let data be written by Arduino to /dev/ttyUSB0
	if (chmod("/dev/ttyUSB0", S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
		gOut << "Cannot Change Permission : /dev/ttyUSB0" << std::endl
			<< "Aquired Data cannot write into /dev/ttyUSB0" << std::endl;
		return;
	}

        m_fd=open("/dev/ttyUSB0",O_RDONLY|O_NOCTTY|O_NDELAY,S_IRWXU | S_IRWXG | S_IRWXO);
//        m_fd=open("/dev/ttyUSB0",O_RDONLY|O_NOCTTY,S_IRWXU | S_IRWXG | S_IRWXO);
        //m_fd=open("~/Desktop/test.txt",O_RDWR);//|O_NOCTTY|O_NDELAY);
        std::cout<<"File Discriptor: "<<m_fd<<std::endl;
/*
        char m;
        std::cin>>m;
        */

        if(m_fd==-1){
            throw std::logic_error("Cannot Open /dev/ttyUSB0, Make sure permission is correct.");
        }

        //Terminal IO Setting
        tcgetattr(m_fd,&m_termOption);
//        cfsetispeed(&m_termOption,B115200);
//        cfsetospeed(&m_termOption,B115200);

        m_termOption.c_ispeed=115200;
        m_termOption.c_ospeed=115200;

        m_termOption.c_cflag&=~PARENB;
        m_termOption.c_cflag&=~CSIZE;
        m_termOption.c_cflag|=CS8;

        //stop bits
        m_termOption.c_cflag&=~CSTOPB;

        //Raw Mode Communication
        m_termOption.c_lflag&=~(ICANON|ECHO|ECHOE|ISIG);
        m_termOption.c_oflag&=~OPOST;

        tcsetattr(m_fd,TCSANOW,&m_termOption);

//        tcflush(m_fd,TCIOFLUSH);
        gOut<<"termios setted."<<std::endl;
}


tvGPIO::~tvGPIO()
{
	gpioTerminate();
        close(m_fd);
        std::cout<<"file closed"<<std::endl;
        gOut<<"Termianl IO port closed"<<std::endl;

	if (chmod("/dev/ttyUSB0", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) < 0) {
		gOut << "Cannot Change Permission : /dev/ttyUSB0" << std::endl
			<< "May have risk that others write into this file." << std::endl;
	}

}

void tvGPIO::ledFlash(const int &ledPinNum, int delaytime)
{
	gpioSetMode(ledPinNum, PI_OUTPUT);

	for (int i = 0; i < 1; i++) {
		gpioWrite(ledPinNum, PI_HIGH);
		gpioDelay(delaytime * 100000);
		gpioWrite(ledPinNum, PI_LOW);
		gpioDelay(delaytime * 100000);
	}
}

std::vector<uint16_t> tvGPIO::daqByNum() const
{
	//std::shared_ptr<uint16_t> vElement(new uint16_t[TC_DNUM]);
        std::vector<uint16_t> vElement(TC_DNUM); 

        /*
        auto recv=read(m_fd,(char*)vElement.get(),TC_DNUM*2);
        std::cout<<"Read Count From /dev/ttyUSB0: "<<recv<<std::endl;

        if(recv<0){
            throw std::logic_error("tvGPIO.cpp line 90, read error");
        };

        for(uint8_t num=0;num<TC_DNUM;num++){
            std::cout<<*(vElement.get()+num)<<std::endl;
        }
        */
        constexpr uint8_t TC_DNUM_BYTES = 2*TC_DNUM;
        char temp[TC_DNUM_BYTES];
        
        memset(temp,0,TC_DNUM_BYTES);
        syncfs(m_fd);

        //flush Serial Port
        //
        
        
        //Read Serial Port
        int ret;
        ret = read(m_fd,temp,TC_DNUM_BYTES);
        std::cout<<"Read Return: "<<ret<<std::endl;

        if(ret==-1){
            std::cout<<"File Discriptor: "<<m_fd<<std::endl;
            throw std::logic_error("Cannot Open /dev/ttyUSB0, Make sure permission is correct.");
        }
        if(ret==0){
            return vElement;
        }

        uint16_t element;
        char b1;
        char b2;
        uint8_t index=0;
        for(uint8_t num=0;num<ret-1;num+=2){
            b1=temp[num];
            b2=temp[num+1];
            element=b2;
            element<<=8;
            element|=b1;
            std::cout<<element<<std::endl;
            vElement[index]=element;
            index++;
        }
	return vElement;
}

void tvGPIO::daqByTime()
{
}
