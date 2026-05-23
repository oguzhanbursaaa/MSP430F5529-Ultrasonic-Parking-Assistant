#include <msp430.h>

// Global variables for distance calculation
volatile unsigned int time_start = 0;
volatile unsigned int time_end = 0;
volatile unsigned int distance_cm = 0;

// Functions
void setup_pins(void);
void setup_timer_capture(void);
void setup_buzzer(void);
void trigger_sensor(void);

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    setup_pins();
    setup_timer_capture();
    setup_buzzer();

    __bis_SR_register(GIE);     // Enable interrupts

    while(1) {
        trigger_sensor();       // Send signal to sensor
        
        __delay_cycles(60000);  // Wait a bit for sensor to read

        // Check distance and turn on LEDs
        if (distance_cm > 30 || distance_cm == 0) {
            // Safe zone
            P3OUT &= ~(BIT0 | BIT1 | BIT2); // LEDs off
            TA0CCR1 = 0;                    // Buzzer off
        } 
        else if (distance_cm > 20 && distance_cm <= 30) {
            // Far 
            P3OUT |= BIT0;                  // Green LED on
            P3OUT &= ~(BIT1 | BIT2);        // Others off

            TA0CCR1 = 500;                  // Buzzer on
            __delay_cycles(100000);         // delay for beep
            TA0CCR1 = 0;                    
            __delay_cycles(400000);         // wait for next beep
        } 
        else if (distance_cm > 10 && distance_cm <= 20) {
            // Middle
            P3OUT |= BIT1;                  // Yellow LED on
            P3OUT &= ~(BIT0 | BIT2);

            TA0CCR1 = 500;                  
            __delay_cycles(100000);         
            TA0CCR1 = 0;                    
            __delay_cycles(150000);         // beep faster
        } 
        else if (distance_cm <= 10) {
            // Very close!
            P3OUT |= BIT2;                  // Red LED on
            P3OUT &= ~(BIT0 | BIT1);

            TA0CCR1 = 500;                  // Buzzer always on
        }
    }
}

void setup_pins(void) {
    // LED pins output
    P3DIR |= (BIT0 | BIT1 | BIT2);
    P3OUT &= ~(BIT0 | BIT1 | BIT2); // all off

    // Trigger pin output
    P1DIR |= BIT6;
    P1OUT &= ~BIT6;

    // Echo pin input
    P2DIR &= ~BIT0;
    P2SEL |= BIT0;  // connect to timer
}

void setup_timer_capture(void) {
    // Timer A1 settings for reading echo
    TA1CCTL1 = CM_3 + CCIS_0 + SCS + CAP + CCIE;
    TA1CTL = TASSEL_2 + MC_2 + TACLR;
}

void setup_buzzer(void) {
    // PWM settings for buzzer
    P1DIR |= BIT2;
    P1SEL |= BIT2;  

    TA0CCR0 = 1000;             
    TA0CCTL1 = OUTMOD_7;        
    TA0CCR1 = 0;                // buzzer is quiet first
    TA0CTL = TASSEL_2 + MC_1 + TACLR; 
}

void trigger_sensor(void) {
    P1OUT |= BIT6;              // make trig pin high
    __delay_cycles(12);         // wait 12us
    P1OUT &= ~BIT6;             // make trig pin low
}

// Interrupt for timer
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer_A1_ISR(void) {
    switch(__even_in_range(TA1IV, 14)) {
        case TA1IV_TACCR1:      
            if (P2IN & BIT0) {
                // signal started
                time_start = TA1CCR1;
            } else {
                // signal ended
                time_end = TA1CCR1;
                
                // calculate distance formula
                if (time_end >= time_start) {
                    distance_cm = (time_end - time_start) / 58; 
                } else {
                    distance_cm = ((65535 - time_start) + time_end) / 58;
                }
            }
            break;
        default:
            break;
    }
}