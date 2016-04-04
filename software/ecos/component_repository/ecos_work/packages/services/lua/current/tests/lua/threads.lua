
function producer(word)
    io.write("Starting producer ", word, "\n");
    while 1 do
        mutex:lock()
        msg = "Msg "..msg_n.." from "..word
        got_msg = 1
        io.write("\nProducer ", word, " sending msg '", msg, "'\n");
        cond:signal()
        msg_n = msg_n + 1
        mutex:unlock()
        if msg_n == 2 then
            return
        end
        thread.delay(50)
    end
end

function consumer(word)
    io.write("Starting consumer ", word, "\n");
    while 1 do
        mutex:lock()
        while got_msg == 0 do
            io.write("Consumer ", word, " waiting for msg\n");
            cond:wait()
        end
        io.write("Consumer ", word, " got msg '", msg, "'\n");
        got_msg = 0
        mutex:unlock()
    end
end

------------------------------------------------------------

function create_prod()
    local t1
    t1 = thread.new(4, producer, {"P"..prod_n})
    t1:resume()
    prod_n = prod_n + 1
end

function create_cons()
    local t1, t2
    t1 = thread.new(4, consumer, {"C1"})
    t2 = thread.new(4, consumer, {"C2"})
    t1:resume()
    t2:resume()
end

------------------------------------------------------------

msg     = ""
got_msg = 0
msg_n   = 0
prod_n  = 0

mutex = mutex.new()
cond  = cond.new(mutex)

ct = thread.new(4, create_prod, {})
ct:resume()
ct = thread.new(4, create_cons, {})
ct:resume()

------------------------------------------------------------

while 1 do
    thread.delay(100)
    if msg_n >= 2 then
        msg_n = 0
        ct = thread.new(4, create_prod, {})
        ct:resume()
    end
end
