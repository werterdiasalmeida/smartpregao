<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html lang="en">
<head>
    <meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta content="" name="description"/>
    <meta content="" name="author"/>

    <!--begin::Global Theme Styles(used by all pages) -->
    <link href="/includes/metronic-6.0.4/css/style.bundle.css" rel="stylesheet" type="text/css"/>
    <!--end::Global Theme Styles -->

    <!--begin::Layout Skins(used by all pages) -->
    <link href="/includes/metronic-6.0.4/css/skins/header/base/easybit.css" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic-6.0.4/css/skins/header/menu/easybit.css" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic-6.0.4/css/skins/brand/easybit.css" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic-6.0.4/css/skins/aside/easybit.css" rel="stylesheet" type="text/css"/>
    <!--end::Layout Skins -->

    <link href="/includes/estilom.css" rel="stylesheet" type="text/css"/>

    <link rel="shortcut icon" href="/favicon.ico"/>

    <title>API</title>
    <style type="text/css">
        body {
            font-family: Arial, Geneva, Verdana, Helvetica, sans-serif;
        }

        ul.rotas {
            list-style-type: none;
            padding: 0;
        }

        ul.rotas > li {
            border: 1px solid #EFEFEF;
            padding: 10px;
            margin-bottom: 5px;
            font-size: 14px;
        }

        ul.rotas > li:hover {
            background-color: #f4fbb9;
        }
    </style>
</head>
<body>
<div class="kt-content  kt-grid__item kt-grid__item--fluid kt-grid kt-grid--hor" id="kt_content">
    <!-- begin:: Content -->
    <div class="kt-container  kt-container--fluid  kt-grid__item kt-grid__item--fluid">
        <h2>Rotas</h2>
        <ul class="rotas">
            <?php foreach ($routes as $route) : ?>
                <?php
                    if(strpos($route, 'clubeold') !== false) :
                        continue;
                    endif;
                ?>
                <li><?= $route ?></li>
            <?php endforeach; ?>
        </ul>
    </div>
</div>
</body>
</html>