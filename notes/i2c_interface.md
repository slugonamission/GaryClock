Gary Clock I2C Semantics
==========================
+ The programmer will connect over I2C as a *slave* device.
    + The clock will already be an I2C master to communicate with the RTC.
+ The programmer will have slave ID `0x28`.
+ On connection, the clock must send a transaction to set the current time to the programmer. The programmer *probably* will not start unless this is sent.
+ The transaction format is `<regnum><data1><data2>...<datan>`. Writes to different registers **must** be contained in seperate transactions. Transaction splitting is not supported.
+ The register file is as follows:

        RegNum         Dir          Data
        0              R/W          The current time of the format <hour><min><second>, all as seperate 8 bit words.
        1              R/W          The current LED state.

+ Current I2C layout map:

        Addr          Usage
        0x28          Programmer
        0x68          RTC (DS1338)
        0x70 - 0x72   7-seg displays
