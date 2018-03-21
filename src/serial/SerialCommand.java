/**
 * @author Adrian Leren
 *
 * Helper class for generating valid serial commands.
 */
package serial;

public class SerialCommand {

	private String command;
	private int value;
	private boolean valid = false;
	
	/**
	 * Create a serial command from a command string.
	 *
	 * @param command The command string to generate a command from.
	 */
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
	
	/**
	 * Create a serial command from a command-and-value pair.
	 * 
	 * @param command The command to use (eg. 'HI', 'RS', 'FS', 'TS', 'RD', 'FD', 'ST')
	 * @param value The value associated to the command and variable to modify on robot.
	 */
	public SerialCommand(String command, int value) {
		this.command = command;
		this.value = value;
		this.valid = true;
	}
	
	
	/**
	 * Chack if this serial command is valid.
	 * 
	 * @return 'true' if this command is valid.
	 */
	public boolean isValid() {
		return this.valid;
	}
	
	
	/**
	 * @return The command section of this serial command.
	 */
	public String getCommand() {
		return this.command;
	}
	
	
	/**
	 * @return The value section of this serial command.
	 */
	public int getValue() {
		return this.value;
	}
}
