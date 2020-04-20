from kafka import KafkaProducer

producer = KafkaProducer(bootstrap_servers=['iotproject.space:9092'])

# Asynchronous by default
future = producer.send('test', b'off')