# Screenshare Tool API
Very simple API which allows you to perform actions in java, which would normally be impossible (ex: String searching, Module enumerating, Process Handle enumerating).

## Information
- Only works on Windows.
- Only works under 64 bit operating systems.
- Requires Java 8.

## How to use:
- Add ScreenshareTool.jar as a dependency to your project.
	```java
	ScreenshareTool sstool = new ScreenshareTool();
	
	int pId = sstool.getProcessByNameAndWindowTitle("javaw.exe", "Minecraft 1.7.10");
	
	if(pId == -1) {
		throw new RuntimeException("An error has occurred while attempting to get the process' pId");
	}
	
	sstool.setSelectedPid(pId);
	
	sstool.searchString(new String[] {"faosijdoiwoai", "MasterManthe", "TehNeon"});
	
	for(String handleName : sstool.getProcessHandles())
		System.out.println(handleName);
	```

## How to compile(source):
- It uses maven to handle it's dependencies.
- Simply make a run configuration on your IDE of your choice and set the default goal to "clean install".

## Upcoming features:
- Process modules enumerating.
- Process handles enumerating.

Credits: Ghose(buzznacker), Fyu.
