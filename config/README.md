##How to do the thing
The file you update is called presets.json

If you run `python check.py` in the same directory as presets.json and as
preset-schema.json, it will check for typos and type errors.  Upload
presets.json to the robot.  Put it in /home/lvuser/presets.json.  Restart
robot code.  When it loads (3 or 4 seconds later), it will read the config
file.

##How to upload
If you have a mac, scp will be the best option.  Type `man scp` for a tutorial
on how to use it but it should look something like the following:

`scp ./presets.json lvuser@roborio-973-frc.local:/home/lvuser/presets.json`  I
didn't test that so if it doesn't work, read the doc.

If you have windows you'll have to download a seperate file transfer utility.
My favorite is FileZilla (I think it's on sloth and pangolin).  Connect to the
robot, find presets.json on the left, and drag it into /home/lvuser on the
right.  It might ask if you want to override, click yes.

##How to restart robot code
There's two possible cases:

###The robot code started automatically when the robot turned on
Yay.  Click through the tabs on the left side of the driver station.  On
either the second or third tab there's a "restart robot code" button.  Click
it 4 or 5 times.  Click it a 6th for good measure.  Sometimes the code doesn't
actually restart, so look at the gyro and make sure it stops reading for a
second (if it stopps reading for a second then starts reading again, you know
the code restarted).

###The robot didn't start automatically
step 1: Putty into the robot as user lvuser.  

step 2: Run the following command:

`./mine`

step 3: If it says something like "HAL FAILED TO INITIALIZE" and dies, goto
step 2 (it will probably fail the first time but sometimes it takes 3 tries)

If it keeps spewing text out forever, that means it's running.  

##How to putty into the robot
Start putty.  It should be pinned to the task bar in sloth and pangolin.  If
you don't have it, download it.

For host name type `roborio-973-frc.local` and click open
When it asks for a username, put `lvuser`
When it asks for a password, just press enter (blank password)
