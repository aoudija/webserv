<?php
session_start();

// Check if the user is not logged in, redirect to login page
if (!isset($_SESSION['user'])) {
    if (!isset($_COOKIE['username'])) {
    header("Location: ../signin.php");
    exit();
    }
}

if(isset($_GET['logout'])) {
    // Destroy session and redirect to login page
    session_destroy();
    setcookie('username', '', time() - 3600, '/');
    header("Location: ../signin.php");
    exit();
}

// Retrieve user information (for demonstration purposes, read from the users.txt file)
$userData = file('../users.txt', FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);

foreach ($userData as $line) {
    list($savedName, $savedUsername, $hashedPassword) = explode('|', $line);

    // Check if the username matches
    if ($_COOKIE['username'] === $savedUsername) {
        $fname = $savedName;
        break;
    }
}
?>


<!DOCTYPE html>
<!-- Website - www.codingnepalweb.com -->
<html lang="en" dir="ltr">
  <head>
    <meta charset="UTF-8" />
    <title>Dashboard</title>
    <link rel="stylesheet" href="style.css" />
    <!-- Boxicons CDN Link -->
    <link href='https://unpkg.com/boxicons@2.1.4/css/boxicons.min.css' rel='stylesheet'>
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  </head>
  <body>
    <div class="sidebar">
      <div class="logo-details">
        <i class="bx bxl-c-plus-plus"></i>
        <span class="logo_name">1337</span>
      </div>
      <ul class="nav-links">
        <li>
          <a href="#" class="active">
            <i class="bx bx-grid-alt"></i>
            <span class="links_name">Dashboard</span>
          </a>
        </li>
        <li>
          <a href="/CGI/index.html">
            <i class="bx bx-box"></i>
            <span class="links_name">HOME</span>
          </a>
        </li>
        <li>
          <a href="/CGI/php.php">
            <i class="bx bxl-php"></i>
            <span class="links_name">PHP</span>
          </a>
        </li>
        <li>
          <a href="/CGI/py.py">
            <i class="bx bxl-python"></i>
            <span class="links_name">Python</span>
          </a>
        </li>
        <li>
          <a href="#">
            <i class="bx bx-coin-stack"></i>
            <span class="links_name">Stock</span>
          </a>
        </li>
        <li>
          <a href="#">
            <i class="bx bx-book-alt"></i>
            <span class="links_name">Total order</span>
          </a>
        </li>
        <li>
          <a href="#">
            <i class="bx bx-user"></i>
            <span class="links_name">Team</span>
          </a>
        </li>
        <li>
          <a href="#">
            <i class="bx bx-message"></i>
            <span class="links_name">Messages</span>
          </a>
        </li>
        <li>
          <a href="#">
            <i class="bx bx-heart"></i>
            <span class="links_name">Favrorites</span>
          </a>
        </li>
        <li>
          <a href="#">
            <i class="bx bx-cog"></i>
            <span class="links_name">Setting</span>
          </a>
        </li>
        <li class="log_out">
          <a href="?logout=logout">
            <i class="bx bx-log-out"></i>
            <span class="links_name">Log out</span>
          </a>
        </li>
      </ul>
    </div>
    <section class="home-section">
      <nav>
        <div class="sidebar-button">
          <i class="bx bx-menu sidebarBtn"></i>
          <span class="dashboard">Dashboard</span>
        </div>
        <div class="search-box">
          <input type="text" placeholder="Search..." />
          <i class="bx bx-search"></i>
        </div>
        <div class="profile-details">
          <img src="images/avatar.jpeg" alt="" />
          <span class="admin_name"><?php if (isset($fname)) echo "$fname"; else  echo "GUEST"?></span>
          <i class="bx bx-chevron-down"></i>
        </div>
      </nav>

      <div class="home-content">
        <div class="overview-boxes">
          <div class="box">
            <div class="right-side">
              <div class="box-topic">Total Order</div>
              <div class="number">40,876</div>
              <div class="indicator">
                <i class="bx bx-up-arrow-alt"></i>
                <span class="text">Up from yesterday</span>
              </div>
            </div>
            <i class="bx bx-cart-alt cart"></i>
          </div>
          <div class="box">
            <div class="right-side">
              <div class="box-topic">Total Sales</div>
              <div class="number">38,876</div>
              <div class="indicator">
                <i class="bx bx-up-arrow-alt"></i>
                <span class="text">Up from yesterday</span>
              </div>
            </div>
            <i class="bx bxs-cart-add cart two"></i>
          </div>
          <div class="box">
            <div class="right-side">
              <div class="box-topic">Total Profit</div>
              <div class="number">$12,876</div>
              <div class="indicator">
                <i class="bx bx-up-arrow-alt"></i>
                <span class="text">Up from yesterday</span>
              </div>
            </div>
            <i class="bx bx-cart cart three"></i>
          </div>
          <div class="box">
            <div class="right-side">
              <div class="box-topic">Total Return</div>
              <div class="number">11,086</div>
              <div class="indicator">
                <i class="bx bx-down-arrow-alt down"></i>
                <span class="text">Down From Today</span>
              </div>
            </div>
            <i class="bx bxs-cart-download cart four"></i>
          </div>
        </div>

        <div class="sales-boxes">
          <div class="recent-sales box">
            <div class="title">Recent Sales</div>
            <div class="sales-details">
              <ul class="details">
                <li class="topic">Date</li>
                <li><a href="#">02 Jan 2021</a></li>
                <li><a href="#">02 Jan 2021</a></li>
                <li><a href="#">02 Jan 2021</a></li>
                <li><a href="#">02 Jan 2021</a></li>
                <li><a href="#">02 Jan 2021</a></li>
                <li><a href="#">02 Jan 2021</a></li>
                <li><a href="#">02 Jan 2021</a></li>
              </ul>
              <ul class="details">
                <li class="topic">Customer</li>
                <li><a href="#">Alex Doe</a></li>
                <li><a href="#">David Mart</a></li>
                <li><a href="#">Roe Parter</a></li>
                <li><a href="#">Diana Penty</a></li>
                <li><a href="#">Martin Paw</a></li>
                <li><a href="#">Doe Alex</a></li>
                <li><a href="#">Aiana Lexa</a></li>
                <li><a href="#">Rexel Mags</a></li>
                <li><a href="#">Tiana Loths</a></li>
              </ul>
              <ul class="details">
                <li class="topic">Sales</li>
                <li><a href="#">Delivered</a></li>
                <li><a href="#">Pending</a></li>
                <li><a href="#">Returned</a></li>
                <li><a href="#">Delivered</a></li>
                <li><a href="#">Pending</a></li>
                <li><a href="#">Returned</a></li>
                <li><a href="#">Delivered</a></li>
                <li><a href="#">Pending</a></li>
                <li><a href="#">Delivered</a></li>
              </ul>
              <ul class="details">
                <li class="topic">Total</li>
                <li><a href="#">$204.98</a></li>
                <li><a href="#">$24.55</a></li>
                <li><a href="#">$25.88</a></li>
                <li><a href="#">$170.66</a></li>
                <li><a href="#">$56.56</a></li>
                <li><a href="#">$44.95</a></li>
                <li><a href="#">$67.33</a></li>
                <li><a href="#">$23.53</a></li>
                <li><a href="#">$46.52</a></li>
              </ul>
            </div>
            <div class="button">
              <a href="#">See All</a>
            </div>
          </div>
          <div class="top-sales box">
            <div class="title">Top Seling Product</div>
            <ul class="top-sales-details">
              <li>
                <a href="#">
                  <img src="images/sunglasses.jpg" alt="" />
                  <span class="product">Vuitton Sunglasses</span>
                </a>
                <span class="price">$1107</span>
              </li>
              <li>
                <a href="#">
                  <img src="images/jeans.jpg" alt="" />
                  <span class="product">Hourglass Jeans </span>
                </a>
                <span class="price">$1567</span>
              </li>
              <li>
                <a href="#">
                  <img src="images/nike-min.jpg" alt="" />
                  <span class="product">Nike Sport Shoe</span>
                </a>
                <span class="price">$1234</span>
              </li>
              <li>
                <a href="#">
                  <img src="images/scarves.jpg" alt="" />
                  <span class="product">Hermes Silk Scarves.</span>
                </a>
                <span class="price">$2312</span>
              </li>
              <li>
                <a href="#">
                  <img src="images/blueBag.jpg" alt="" />
                  <span class="product">Succi Ladies Bag</span>
                </a>
                <span class="price">$1456</span>
              </li>
              <li>
                <a href="#">
                  <img src="images/bag.jpg" alt="" />
                  <span class="product">Gucci Womens's Bags</span>
                </a>
                <span class="price">$2345</span>
              </li>

              <li>
                <a href="#">
                  <img src="images/addidas.jpg" alt="" />
                  <span class="product">Addidas Running Shoe</span>
                </a>
                <span class="price">$2345</span>
              </li>
              <li>
                <a href="#">
                  <img src="images/shirt.jpg" alt="" />
                  <span class="product">Bilack Wear's Shirt</span>
                </a>
                <span class="price">$1245</span>
              </li>
            </ul>
          </div>
        </div>
      </div>
    </section>

    <script>
      let sidebar = document.querySelector(".sidebar");
      let sidebarBtn = document.querySelector(".sidebarBtn");
      sidebarBtn.onclick = function () {
        sidebar.classList.toggle("active");
        if (sidebar.classList.contains("active")) {
          sidebarBtn.classList.replace("bx-menu", "bx-menu-alt-right");
        } else sidebarBtn.classList.replace("bx-menu-alt-right", "bx-menu");
      };
    </script>
  </body>
</html>
