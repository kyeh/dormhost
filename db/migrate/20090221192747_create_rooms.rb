class CreateRooms < ActiveRecord::Migration
  def self.up
    create_table :rooms do |t|
      t.column  :host_id,           :integer, :null => false
      t.column  :college_id,        :integer, :null => false
      t.column  :street_address_1,  :string
      t.column  :street_address_2,  :string
      t.column  :city,              :string,  :limit => 50
      t.column  :state,             :string,  :limit => 2
      t.column  :zip,               :string,  :limit => 12
      t.column  :active,            :boolean, :default => 0 

      t.timestamps
    end
  end

  def self.down
    drop_table :rooms
  end
end
