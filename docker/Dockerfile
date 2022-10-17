FROM php:7.3-apache
RUN apt-get update && apt-get install -y \
	apt-transport-https \
	libpq-dev \
	libfreetype6-dev \
	libjpeg62-turbo-dev \
    libmcrypt-dev \
    libgmp-dev re2c libmhash-dev file \
    zlib1g-dev \
    libzip-dev \
    unzip \ 
	&& docker-php-ext-install pdo pdo_pgsql pgsql calendar \
	&& docker-php-ext-install -j$(nproc) iconv \
    && docker-php-ext-configure gd --with-freetype-dir=/usr/include/ --with-jpeg-dir=/usr/include/ \
    && docker-php-ext-install -j$(nproc) gd \
    && docker-php-ext-install bcmath

RUN ln -s /usr/include/x86_64-linux-gnu/gmp.h /usr/local/include/
RUN docker-php-ext-configure gmp
RUN docker-php-ext-install gmp
RUN docker-php-ext-install zip
RUN pecl install mcrypt-1.0.3
RUN docker-php-ext-enable mcrypt

RUN a2enmod headers

#RUN pecl install xdebug
#RUN docker-php-ext-enable xdebug
#COPY xdebug.ini /usr/local/etc/php/conf.d/xdebug.ini

# RUN apt-get install -y wkhtmltopdf
RUN apt-get install wget -y \
    libxrender1 \
    libfontconfig1 \
    libxext6
RUN wget https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.4/wkhtmltox-0.12.4_linux-generic-amd64.tar.xz -P /opt/
RUN tar -xf /opt/wkhtmltox-0.12.4_linux-generic-amd64.tar.xz -C /opt/ && rm -r /opt/wkhtmltox-0.12.4_linux-generic-amd64.tar.xz
RUN chown -R root. /opt/wkhtmltox/
RUN ln -s /opt/wkhtmltox/bin/wkhtmltopdf /usr/bin/

COPY server.crt /etc/apache2/ssl/server.crt
COPY server.key /etc/apache2/ssl/server.key
COPY apache_default /etc/apache2/sites-enabled/000-default.conf

# Agendamento de cron SIMEC
RUN apt-get update && apt-get -y install cron
RUN crontab -l | { cat; echo "* * * * * /usr/bin/wget -qO- http://localhost/seguranca/agendamentoscriptsexecutar.php >> /var/log/cron.log 2>&1"; } | crontab -

RUN a2enmod rewrite
RUN a2enmod ssl
RUN a2enmod proxy
RUN a2enmod proxy_http
RUN service apache2 restart

WORKDIR /var/www

EXPOSE 80 443 4433

RUN sed -i 's/^exec /service cron start\n\nexec /' /usr/local/bin/apache2-foreground