<?php
session_start();
if (isset($_SESSION['user'])) {
    header("Location: admin.php");
    exit();
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {

    if (isset($_POST['username']) && isset($_POST['password'])){
        $username = $_POST['username'];
        $password = $_POST['password'];
        // Read user data from the text file (for demonstration purposes)
        $filePath = 'users.txt';
    
        if (!file_exists($filePath)) {
            // Create the file with a default user for protection (replace with your actual default user)
            $defaultUser = "admin|admin|". password_hash("admin", PASSWORD_BCRYPT) . "\n";
            file_put_contents($filePath, $defaultUser);
        }
        $userData = file($filePath, FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
    
        foreach ($userData as $line) {
            list($savedName, $savedUsername, $hashedPassword) = explode('|', $line);
    
            // Check if the username matches
            if ($username === $savedUsername) {
                // Check if the password is correct
                if (password_verify($password, $hashedPassword)) {
                    // Valid login, set session and redirect to admin page or any other destination
                    $_SESSION['user'] = $username;
                    setcookie('username', $username, time() + (86400 * 30), '/'); // 86400 = 1 day
                    header('Location: admin.php');
                    exit();
                } else {
                    $error = 'Invalid password';
                }
            }
        }
    
        // If no matching user was found
        $error = 'Invalid username';
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login Page</title>
    <link rel="stylesheet" href="style.css">
    <!-- <link rel="stylesheet" href="style1.css"> -->
</head>
<body class="login">
<nav class="navMenu">
<a href="/CGI/">Home</a>
<a href="admin.php">Welcome</a>
<a href="login.php">Login</a>
<a href="signup.php">Signup</a>
    </nav>
    <div class="container login">
        <h2>Login</h2>
        <?php if(isset($error)) { echo "<p class='error'>$error</p>"; } ?>
        <form method="post" action="">
            <label for="username">Username:</label>
            <input type="text" name="username" required>
            <label for="password">Password:</label>
            <input type="password" name="password" required>
            <input type="submit" value="Login">
        </form>
        <p id="ty">Don't have an account?</p>
        <p id="ty"><a href="signup.php">Sign up here</a></p>
    </div>
</body>
</html>
