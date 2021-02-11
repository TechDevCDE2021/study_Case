#! /bin/sh
mkdir -p public/js/lib
mkdir -p public/styles/lib

cp node_modules/socket.io/client-dist/socket.io.min.* public/js/lib
cp node_modules/@mongodb-js/charts-embed-dom/dist/charts-embed-dom.umd.min.* public/js/lib
cp node_modules/axios/dist/axios.min.* public/js/lib
cp node_modules/bootstrap/dist/js/bootstrap.bundle.min.* public/js/lib
cp node_modules/jquery/dist/jquery.min.* public/js/lib
cp node_modules/startbootstrap-sb-admin/dist/js/scripts.js public/js/lib
cp node_modules/@fortawesome/fontawesome-free/js/all.min.js public/js/lib

cp node_modules/bootstrap:dist/css/bootstrap.min.* public/styles/lib
cp node_modules/startbootstrap-sb-admin/dist/css/styles.css public/styles/lib
cp node_modules/@fortawesome/fontawesome-free/css/all.min.css public/styles/lib
