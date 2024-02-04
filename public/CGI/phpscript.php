<?php
    if ($_SERVER['REQUEST_METHOD'] === "GET"){
        if (isset($_GET["name"]) && isset($_GET["last_name"])){
            $name = $_GET["name"];
            $last_name = $_GET["last_name"];
        }
        else{
            header("Location: phpget.html");
            echo"<!DOCTYPE html>
            <html lang='en'>
            <head>

            </head>
            <body>
 
            </body>
            </html>
            ";
            exit();
        }
        $title = "PHP GET";
        $req = "get";
    }
    else{
        if (isset($_POST["name"]) && isset($_POST["last_name"])){
            $name = $_POST["name"];
            $last_name = $_POST["last_name"];
        }
        else{
            header("Location: phppost.html");
            exit();
        }
        $title = "PHP POST";
        $req = "post";
    }
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title><?php echo $title?></title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <nav class="navMenu">
        <a href="/CGI/">Home</a>
        <a href="phpget.html">PHP/GET</a>
        <a href="phppost.html">PHP/POST</a>
    </nav>
    <div class="container">
        <h1><?php echo $title?></h1>
        <p id="php">First Name: <span><?php echo $name?></span></p>
        <p id="php">Last Name : <span><?php echo $last_name?></span></p>
        <form method=<?php echo $req?>>
            <input type="submit" name="logout" value="GO BACK">
        </form>
    </div>
</body>
</html>
