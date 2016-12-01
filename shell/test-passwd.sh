# Read secret string                                                                                                                                                              
read_secret()
{
    # Disable echo.
    stty -echo

    # Set up trap to ensure echo is enabled before exiting if the script
    # is terminated while echo is disabled.
    trap 'stty echo; echo' EXIT

    # Read secret.
    read "$@"

    # Enable echo.
    stty echo
    trap - EXIT

    # Print a newline because the newline entered by the user after
    # entering the passcode is not echoed. This ensures that the
    # next line of output begins at a new line.
    echo
}

while true; do
  echo -n "Please enter your user's password: "
  read_secret userpasswd
  echo -n "Please confirm your user's password: "
  read_secret confpasswd
  [ "x${userpasswd}" = "x${confpasswd}" ] && break
  echo "Two passwords are inconsistent, please retry again"
done

echo ${userpasswd}
