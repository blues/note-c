. venv/bin/activate
echo $MD5SRV_TOKEN
python3 ./test/hitl/scripts/md5srv.py --dir md5srv-files --save > md5srv.log 2>&1 &
MD5SRV_PID=$!
echo "MD5SRV_PID=$MD5SRV_PID" >> $GITHUB_ENV

# Prevent the action from thinking we exited early
sleep 2           
