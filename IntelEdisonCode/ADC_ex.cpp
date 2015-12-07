#include "mraa.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "SparkFunADS1015.h"

using namespace std;

// Declare a variable for our i2c object. You can create an
//  arbitrary number of these, and pass them to however many
//  slave devices you wish.
mraa::I2c* adc_i2c;

int running = 0;
// CTRL+C
void sig_handler(int signo)
{
    if ( signo == SIGINT ) {
        printf("Closing ADC\n");
        running = -1;
    }
}

int main()
{
        // Initialize the GPIO pins 14,15,49
        mraa_init();
        mraa_gpio_context GP14 = mraa_gpio_init(36);
        mraa_gpio_dir(GP14, MRAA_GPIO_OUT_LOW);
        mraa_gpio_context GP15 = mraa_gpio_init(48);
        mraa_gpio_dir(GP15, MRAA_GPIO_OUT_LOW);
        mraa_gpio_context GP49 = mraa_gpio_init(47);
        // Create a FILE HANDLER
        ofstream ADCOutput;
        // The ADC is (by default) connected to I2C channel 1. Here, we create
        //  a device to pass to the ads1015 object constructor.
        adc_i2c = new mraa::I2c(1);

        // Now, pass that I2C object and the address of the ADC block in your
        //  system to the ads1015 object constructor. Note that there are up to
        //  four different addresses available here, settable by jumper on the
        //  board. You'll need to create an ads1015 object for each one.
        ads1015 adc(adc_i2c, 0x48);

        // There are 6 settable ranges:
        //  _0_256V - Range is -0.256V to 0.255875V, and step size is 125uV.
        //  _0_512V - Range is -0.512V to 0.51175V, and step size is 250uV.
        //  _1_024V - Range is -1.024V to 1.0235V, and step size is 500uV.
        //  _2_048V - Range is -2.048V to 2.047V, and step size is 1mV.
         //  _4_096V - Range is -4.096V to 4.094V, and step size is 2mV.
        //  _6_144V - Range is -6.144V to 6.141V, and step size is 3mV.
        // The default setting is _2_048V.
        // NB!!! Just because FS reading is > 3.3V doesn't mean you can take an
        //  input above 3.3V! Keep your input voltages below 3.3V to avoid damage!
        adc.setRange(_2_048V);

        // getResult() returns a normalized floating point value representing the
        //  current voltage of the passed channel. User is responsible for
        //  logic to determine whether the value is at min or max.
        // cout<<"Ch 0: "<<adc.getResult(0)<<endl;

        double adcresult = 0.0;
        /* READ ADC input forever until SIGINT (Ctrl+c) */
        while ( running == 0 ) {

               if(!(adc.getResult(0) < 0)){
               /* Read adc input */
                ADCOutput.open("ADCOUT.txt");
                ADCOutput << setprecision(2)<<fixed;
                ADCOutput << 100 * adc.getResult(0)<<endl;
                ADCOutput.close();

                cout << setprecision(2)<<fixed;
                adcresult = adc.getResult(0);
                cout<<"Force [lbs]: "<< 100 * adc.getResult(0)<<endl;
               }
                /* Check value of ADC and change LED*/
                if(adcresult < 0.028){
                    mraa_gpio_write(GP14, 1);
                    mraa_gpio_write(GP15, 1);
                    mraa_gpio_write(GP49, 1);
                 }else if(adcresult > 0.028 && adcresult < 0.057){
                    mraa_gpio_write(GP14, 1);
                    mraa_gpio_write(GP15, 0);
                    mraa_gpio_write(GP49, 0);
                 }else if(adcresult > 0.057 && adcresult < 0.085){
                     mraa_gpio_write(GP14, 1);
                    mraa_gpio_write(GP15, 1);
                     mraa_gpio_write(GP49, 0);
                 }else if(adcresult > 0.085 && adcresult < 0.114){
                     mraa_gpio_write(GP14, 0);
                     mraa_gpio_write(GP15, 1);
                     mraa_gpio_write(GP49, 0);
                 }else if(adcresult > 0.114 && adcresult < 0.143){
                     mraa_gpio_write(GP14, 0);
                     mraa_gpio_write(GP15, 1);
                     mraa_gpio_write(GP49, 1);
                 }else if(adcresult > 0.143 && adcresult < 0.172){
                     mraa_gpio_write(GP14, 0);
                     mraa_gpio_write(GP15, 0);
                     mraa_gpio_write(GP49, 1);
                 }else if(adcresult > 0.172){
                     mraa_gpio_write(GP14, 1);
                     mraa_gpio_write(GP15, 0);
                     mraa_gpio_write(GP49, 1);
                 }

               /* Sleep for 1 second */
                sleep(1);
        }

        return 0;



}
                                              



