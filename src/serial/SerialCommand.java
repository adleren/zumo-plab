package serial;

public class SerialCommand {

	private String command;
	private int value;
	private boolean valid = false;
	
	public SerialCommand(String command) {
		try {
			String[] parts = command.split(":");
			this.command = parts[0];
			
			if (parts[1].contains("#"))
				parts[1] = parts[1].substring(0, parts[1].length() - 1);
			
			this.value = Integer.parseInt(parts[1]);
			this.valid = true;
		} catch (Exception e) {
			e.printStackTrace();
			this.valid = false;
		}
	}
	
	public SerialCommand(String command, int value) {
		this.command = command;
		this.value = value;
		this.valid = true;
	}
	
	public boolean isValid() {
		return this.valid;
	}
	
	public String getCommand() {
		return this.command;
	}
	
	public int getValue() {
		return this.value;
	}
}
