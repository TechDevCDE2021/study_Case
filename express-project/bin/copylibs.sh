#! /bin/sh
mkdir -p public/js/lib
cp node_modules/socket.io/client-dist/socket.io.min.js* public/js/lib
cp node_modules/@mongodb-js/charts-embed-dom/dist/charts-embed-dom.umd.min.js* public/js/lib
cp node_modules/axios/dist/axios.min.js* public/js/lib
