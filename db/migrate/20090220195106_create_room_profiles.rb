class CreateRoomProfiles < ActiveRecord::Migration
  def self.up
    create_table :room_profiles do |t|

      t.timestamps
    end
  end

  def self.down
    drop_table :room_profiles
  end
end
