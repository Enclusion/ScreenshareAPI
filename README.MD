# Screenshare Tool API
Very simple API which allows you to perform actions in java, which would normally be impossible (ex: String searching, Module enumerating, Process Handle enumerating).

## Information
- Only works on Windows;
- Only works under 64 bit operating systems.
- Requires Java 8.

## Usage example:
	```java
	ScreenshareTool sstool = new ScreenshareTool();
	
	int pId = sstool.getProcIdByWindowName("Minecraft 1.7.10");
	
	if(pId == -1)
		throw new RuntimeException("An error has occurred while attempting to get the process' pId");
	
	sstool.setpId(pId);
	
	sstool.searchString(new String[] {"faosijdoiwoai", "MasterManthe", "TehNeon"});
	
	```

## How to compile(source):
- It uses maven to handle it's dependencies.
- Simply make a run configuration on your IDE of your choice and set the default goal to "clean install"

Credits: Ghose(buzznacker), Fyu.
