<?php
session_start();
if (isset($_SESSION['user'])) {
    header("Location: admin.php");
    exit();
}
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $queryString = $_SERVER['QUERY_STRING'];
    parse_str($queryString, $_POST);
    // Validate the form data (you can add more validation as needed)
    $name = $_POST['name'];
    $username = $_POST['username'];
    $password = $_POST['password'];


    // Hash the password (use a secure hashing algorithm like bcrypt)
    

    // Save user information to a text file (for demonstration purposes)
    $filePath = 'users.txt';
    
    if (!file_exists($filePath)) {
        // Create the file with a default user for protection (replace with your actual default user)
        $defaultUser = "admin|admin|". password_hash("admin", PASSWORD_BCRYPT) . "\n";
        file_put_contents($filePath, $defaultUser);
    }
    $userData = file('users.txt', FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);

    foreach ($userData as $line) {
        list(, $savedUsername, ) = explode('|', $line);
        
        if ($username === $savedUsername) {
            $error = 'Username already exists.';
            break;
        }
    }
    if (!isset($error)) {
    $hashedPassword = password_hash($password, PASSWORD_BCRYPT);
    $userData = "$name|$username|$hashedPassword" . PHP_EOL;
    file_put_contents('users.txt', $userData, FILE_APPEND);

    // Redirect to login page or any other destination
    header('Location: login.php');
    exit();
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Signup Page</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
<!-- <div class="container">
    <ul class="centered-list">
  <li><a href="admin.php">Home</a></li>
  <li><a href="login.php">Login</a></li>
  <li><a href="signup.php">Signup</a></li>
  <li style="float:right"><a class="active" href="#about">About</a></li>
</ul>
</div> -->

<nav class="navMenu">
<a href="/CGI/">Home</a>
<a href="admin.php">Welcome</a>
<a href="login.php">Login</a>
<a href="signup.php">Signup</a>
    </nav>
    <div class="container login">
        <h2>Signup</h2>
        <?php if (isset($error)) { echo "<p class='error'>$error</p>"; } ?>
        <form method="post" action="">
            <label for="name">Name:</label>
            <input type="text" name="name" required>
            <label for="username">Username:</label>
            <input type="text" name="username" required>
            <label for="password">Password:</label>
            <input type="password" name="password" required>
            <input type="submit" value="Signup">
        </form>
        <p id="ty">Already have an account?</p>
        <p id="ty"><a href="login.php">Login here</a></p>
    </div>
</body>
</html>
