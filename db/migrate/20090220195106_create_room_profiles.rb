class CreateRoomProfiles < ActiveRecord::Migration
  def self.up
    create_table :room_profiles do |t|
      t.column  :room_id,       :integer, :null => false
      t.column  :level_id,      :integer
      t.column  :type,          :string,  :limit => 50
      t.column  :bed_type,      :string,  :limit => 50
      t.column  :max_capacity,  :integer
      t.column  :pets,          :string,  :limit => 50
      t.column  :food,          :string,  :limit => 100
      t.column  :roommates,     :integer
      t.column  :internet,      :boolean, :default => false
      t.column  :on_campus,     :boolean, :default => false
      t.column  :cost,          :decimal, :precision => 5,  :scale => 2
      t.column  :notes          :string
      
      t.timestamps
    end
  end

  def self.down
    drop_table :room_profiles
  end
end
