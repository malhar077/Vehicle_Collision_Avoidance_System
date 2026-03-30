CC = gcc
CFLAGS = -Wall -lpthread -Iinclude

SRC = src/main_controller.c
IPC = ipc/*.c
VEHICLE = src/vehicle_process.c
CREATE = src/create_vehicle.c

TARGET = main_controller
VEHICLE_BIN = vehicle_process
CREATE_BIN = create_vehicle

all:
	$(CC) $(SRC) $(IPC) -o $(TARGET) $(CFLAGS)
	$(CC) $(VEHICLE) -o $(VEHICLE_BIN)
	$(CC) $(CREATE) -o $(CREATE_BIN)
	@echo "✅ Build complete"


run: all
	@echo "🚀 Running system..."
	./$(TARGET) & \
	sleep 1; \
	./$(CREATE_BIN)


test: all
	./$(TARGET)


stop:
	pkill main_controller || true
	pkill vehicle_process || true
	pkill create_vehicle || true
	@echo "🛑 Stopped all"


clean:
	rm -f $(TARGET) $(VEHICLE_BIN) $(CREATE_BIN)
	@echo "🧹 Cleaned"