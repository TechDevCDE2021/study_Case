mkdir public\js\lib
mkdir public\styles\lib

copy /Y /V node_modules\socket.io\client-dist\socket.io.min.js* public\js\lib\
copy /Y /V node_modules\@mongodb-js\charts-embed-dom\dist\charts-embed-dom.umd.min.js* public\js\lib\
copy /Y /V node_modules\axios\dist\axios.min.* public\js\lib\
copy /Y /V node_modules\bootstrap\dist\js\bootstrap.bundle.min.* public\js\lib\
copy /Y /V node_modules\jquery\dist\jquery.min.* public\js\lib\

copy /Y /V node_modules\bootstrap\dist\css\bootstrap.min* public\styles\lib\