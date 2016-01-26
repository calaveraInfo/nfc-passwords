# NFC passwords

Yet another solution for the password madness: we all hate passwords and they also provide lousy security,  but getting rid of them is exceptionally hard. 

## What's available
Best attempt yet are software password managers, but they have their issues too. For example they can't be used for system logins and it's also frustrating when user moves between many different ‎workstations and operation systems without preinstalled manager. Therefore I believe that good password solution has to be hardware based.

Phones are a good platform, but they won't write a password for you so having a really strong password is a pain in the ass again.‎ Best solution yet for me is a device like Mooltipass, identifying itself as a HID keyboard, eliminating the need to write anything. But even this device has it's issues.

Although they added a password synchronization feature , it still needs a public Internet connection and also an all seein eye cloud account. There is also no falback if you don't have a computer around which is a pretty common situation in the mobile era.

## What I want
What I always wanted was an equivalent of a real physical key for the digital world. I tried to build a smaller form factor of Mooltipass, but making it small enough to connect it to my keyring was too hard for hobbyist like me. But I realized that what I need to carry on my keyring is just the password itself, not the whole device.

‎I tested the SD card as a storage but it didn't feel right and then it hit me. Why not use NFC tags to store the passwords? They are extremely cheap, small enough (especially in sticker form factor) and may be also read (and written) by mobile phones. 

I quickly hacked together a mobile app that allowed me‎ to write my password to a tag and an Arduino based reader that read the tag and input it to a PC as if it is written on a keyboard. And it worked!

Of course there is a catch, NFC tag doesn't have any security so storing an unencrypted password on it is not an option. Luckily there are smartcards (which is in fact a hardware encryption module) that works over the NFC too, like Yubikey NEO.

For realy strong security we can use asymetric encryption‎, so even if the tags are written by mobile phone, allowing us to create nice UI for password input, the phone itself is not a security weak point because it knows just a public key.

## Design overview
My current idea of the whole process is this: User inputs or generates his password in mobile app. The app encrypts the password using a public key and stores it to NFC tag. For using the password, the NFC tag is read by the hardware reader. Then the user has to provide his smart card which decrypts the password and the reader sends it to computer as if it writen on keyboard. Just two simple physical moves with a keyring.

There are a lot of possible combinations, like showing the password directly on mobile phone without a reader. Or the physical tag can be eliminated completely, because  the mobile app can emulate it and so on. There are a lot of scenarios which can be accomplished by a combination of described features.

We have all the ingredients for a holy grail of password management, we just have to make it happen. Since manipulating the NFC smart cards and asymmetric security is a delicate task, this is not a simple hacking anymore so I created this repo which should contain all the code and hardware desing for reader and mobile apps.
