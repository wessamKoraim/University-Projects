module SRAM_controller_1( i_clock, i_startOperation, o_read, o_write, o_address, o_redLed, o_greenLed, io_data);


	parameter ADDRESS_LINES 	= 8;
	parameter DATA_WIDTH			= 8;
	
	
	//States
	
	parameter WRITE				= 0;
	parameter READ					= 1;
	parameter DONE					= 2;
	parameter ERROR				= 3;
	parameter MANAGE_WRITE		= 4;
	parameter MANAGE_READ		= 5;
	parameter INC_ADDRESS		= 6;
	
	
	input 							i_clock;
	input								i_startOperation;
						       
	output							o_read; 
	output  							o_write;
	output[ADDRESS_LINES-1:0]	o_address;
	output							o_redLed;
	output							o_greenLed;
						       
	inout[DATA_WIDTH-1:0]		io_data;
						       
	wire								i_clock;
	wire								i_startOperation;
	                           
	wire[DATA_WIDTH-1:0] 		io_data;
	wire[ADDRESS_LINES-1:0]		o_address;
	
	                          
	reg								o_read;
	reg								o_write;
	reg								o_redLed;
	reg								o_greenLed;
		
	wire								overFlow;
	wire								oddAddress;
			
	reg[2:0]							state;
	reg[2:0]							oldState;
	reg								counter;
	
	reg[DATA_WIDTH-1:0] 			readData;
	reg[DATA_WIDTH-1:0] 			writeData;
	reg[ADDRESS_LINES:0]			modifiedAddress;
	
	assign overFlow 	= modifiedAddress[ADDRESS_LINES];
	assign oddAddress 	= modifiedAddress[0];
	
	assign io_data 		= (~o_write) ? writeData : 8'bz;
	
	assign o_address	=	 modifiedAddress[ADDRESS_LINES-1:0];

	always @ (posedge i_clock)
	begin
		
		if(~i_startOperation)
		begin
		
			o_redLed				<= 1'b0;
			o_greenLed			<= 1'b0;
				
			o_read 				<= 1'b1;
			o_write 				<= 1'b1;
			
			modifiedAddress 	<= 9'b0;
			
			readData				<= 8'b0;
			writeData   		<= 8'b0;
			
			counter				<= 1'b0;
			
			state					<= WRITE;
			
		end
		
		else
		begin
		
			case(state)
			
				WRITE:
				begin
					
					if(overFlow==1'b0)
					begin
					
						o_write		<= 1'b0;
						writeData 	<= (oddAddress==1'b1) ? 8'b10101010 : 8'b01010101;
						oldState		<= WRITE;
						state 		<= MANAGE_WRITE;
						
					end
					
					else
					begin
					
						state 				<= READ;
						modifiedAddress 	<= 9'b0;
					
					end
				
				end
				
				MANAGE_WRITE:
				begin
				
					o_write		<= 1'b1;
					state			<= INC_ADDRESS;
				
				end
				
				INC_ADDRESS:
				begin
				
					modifiedAddress	<= modifiedAddress + 1'b1;
					state					<= oldState;
				
				end
				
				READ:
				begin
				
					if(overFlow==1'b0)
					begin
					
						o_read			<= 1'b0;
						readData		<= io_data;
						state			<= MANAGE_READ;
						oldState		<= READ;
						
					end
					
					else
					begin
						
						state					<= DONE;
						modifiedAddress	<= 9'b0;
					
					end
				
				end
				
				MANAGE_READ:
				begin
				
					counter	<= counter + 1'b1;
					
					if( (oddAddress == 1'b1 && readData == 8'b10101010) || (oddAddress == 1'b0 && readData == 8'b01010101) )
					begin
					
						state 		<= INC_ADDRESS;
						counter		<= 1'b0;
					
					end
					
					else if(counter < 1'b1)
					begin
					
						state	<= READ;
					
					end
					
					else
					begin
					
						state	<= ERROR;
					
					end
				
				end
				
				DONE:
				begin
				
					o_greenLed	<= 1'b1;
					o_read		<= 1'b1;
				
				end
				
				ERROR:
				begin
				
					o_redLed		<= 1'b1;
					o_read		<= 1'b1;
				
				end
				
			endcase
		
		end
		
	end

endmodule