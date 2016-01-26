# NFC passwords

Yet another solution to the "password madness"

We all hate passwords and they also provide lousy security,  but getting rid of them is exceptionally hard.

Best attempt yet are software password managers, but they have their issues too. For example they can't be used for system logins and it's also frustrating when user moves between many different 

Phones are a good platform, but they won't write a password for you so having a really strong password is a pain in the ass again.

Although they added a password synchronization feature

What I always wanted was an equivalent of a real physical key for the digital world. I tried to build a smaller form factor of Mooltipass, but making it small enough to connect it to my keyring was too hard for hobbyist like me. But I realized that what I need to carry on my keyring is just the password itself, not the whole device.

I quickly hacked together a mobile app that allowed me

Of course there is a catch, NFC tag doesn't have any security so storing an unencrypted password on it is not an option. Luckily there are smartcards (which is in fact a hardware encryption module) that works over the NFC too, like Yubikey NEO.

For realy strong security we can use asymetric encryption

My current idea of the whole process is this: User inputs or generates his password in mobile app. The app encrypts the password using a public password and stores it to NFC tag. For using the password, the NFC tag is read by the hardware reader. Then user has to provide his smart card which decrypts the password and the reader sends it to computer as if it writen on keyboard. Just two simple physical moves with a keyring.

There are a lot of possible combinations, like showing the password directly on mobile phone without a reader. Or the physical tag can be eliminated completely, because  the mobile app can emulate it and so on. There are a lot of scenarios which can be accomplished by a combination of described features.

We have all the ingredients for a holy grail of password management, we just have to make it happen. Since manipulating the NFC smart cards and asymmetric security is a delicate task, this is not a simple hacking anymore so I created this repo which should contain all the code and hardware desing for reader and mobile apps.
