class ChangeCost < ActiveRecord::Migration
  def self.up
    remove_column :room_profiles, :cost
    add_column :room_profiles, :cost, :decimal, :precision => 5,  :scale => 2
  end

  def self.down
  end
end
