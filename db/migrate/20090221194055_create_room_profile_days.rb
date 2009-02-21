class CreateRoomProfileDays < ActiveRecord::Migration
  def self.up
    create_table :room_profile_days do |t|
      t.column  :day_id,          :integer, :null => false
      t.column  :room_profile_id, :integer, :null => false

      t.timestamps
    end
  end

  def self.down
    drop_table :room_profile_days
  end
end
