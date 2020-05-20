from kafka import KafkaProducer

producer = KafkaProducer(bootstrap_servers=['34.73.33.253:9092'])

# Asynchronous by default
future = producer.send('test', b'off')