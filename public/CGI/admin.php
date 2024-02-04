<?php
session_start();

// Check if the user is not logged in, redirect to login page
if (!isset($_SESSION['user'])) {
    if (!isset($_COOKIE['username'])) {
    header("Location: login.php");
    exit();
    }
}

if(isset($_POST['logout'])) {
    // Destroy session and redirect to login page
    session_destroy();
    setcookie('username', '', time() - 3600, '/');
    header("Location: login.php");
    exit();
}

// Retrieve user information (for demonstration purposes, read from the users.txt file)
$userData = file('users.txt', FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);

foreach ($userData as $line) {
    list($savedName, $savedUsername, $hashedPassword) = explode('|', $line);

    // Check if the username matches
    if ($_COOKIE['username'] === $savedUsername) {
        $welcomeMessage = "Welcome, $savedName!";
        break;
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Welcome Page</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
<nav class="navMenu">
<a href="/CGI/">Home</a>
<a href="admin.php">Welcome</a>
<a href="login.php">Login</a>
<a href="signup.php">Signup</a>
    </nav>
    <div class="container">
        <?php if (isset($welcomeMessage)) echo "<p>$welcomeMessage</p>"; ?>
        <form method="post">
            <input type="submit" name="logout" value="Logout">
        </form>
    </div>
</body>
</html>
